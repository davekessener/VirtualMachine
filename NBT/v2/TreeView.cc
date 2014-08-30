#include <sstream>
#include <stack>
#include <functional>
#include "TreeView.h"
#include <nbt/NBT.h>
#include <dav/Logger.h>
#include <aux>
#include "Terminal.h"
#include "Node.h"
#include "NBTNode.h"
#include "FileSystem.h"
#include "ViewBuffer.h"

#define MXT_TABSTOP 4

using display::Terminal;

// # ---------------------------------------------------------------------------

struct TreeView::Impl
{
	void display( ) const;
	void up( );
	void down( );
	void in( );
	void out( );
	void expand( );
	Node_ptr cur( ) { return static_cast<bool>(current_) ? current_->current() : root_; }
	void recalculate( );
	int calcHeight( ) const;
	void updateView( );
	void dirty(bool f = true);
	const std::string& getStatus( ) const;

	int x1_, y1_, x2_, y2_;
	int dx_, dy_;
	Object_ptr obj_;
	Node_ptr root_, current_;
	struct { int x, y; } pos_;
	bool dirty_, needRecalculate_;
	mutable struct { bool dirty; std::string msg; } status_;
	ViewBuffer view_;
};

void TreeView::Impl::dirty(bool f)
{
	if(dirty_ != f) status_.dirty = true;
	
	if((dirty_ = f))
	{
		if(static_cast<bool>(current_) && !current_->hasChildren()) out();
		updateView();
		needRecalculate_ = true;
	}
}

// # ===========================================================================

TreeView::TreeView(int x, int y, int w, int h, Object_ptr o) : impl_(new Impl)
{
	impl_->x1_ = x;
	impl_->y1_ = y;
	impl_->x2_ = x + w;
	impl_->y2_ = y + h - 1;
	impl_->dx_ = 0;
	impl_->dy_ = 0;
	impl_->obj_ = o;
	impl_->root_ = o->get();
	impl_->dirty_ = false;
	impl_->status_.dirty = true;
	impl_->needRecalculate_ = true;

	assert(x>=0&&y>=0&&w>0&&h>0);

	impl_->recalculate();
	impl_->updateView();
}

TreeView::~TreeView(void) noexcept
{
	delete impl_;
	LOG("Destructor");
}

bool TreeView::isModified(void) const
{
	return impl_->dirty_;
}

Object_ptr TreeView::getObject(void)
{
	return impl_->obj_;
}

Node_ptr TreeView::getNode(void)
{
	return impl_->cur();
}

void TreeView::modify(bool v)
{
	impl_->dirty(v);
}

// # ---------------------------------------------------------------------------

void TreeView::i_doRender(void) const
{
	impl_->recalculate();

	impl_->display();

	Terminal::instance().setCursorPos(
		impl_->x1_ + impl_->dx_ + impl_->pos_.x * MXT_TABSTOP + 1,
		impl_->y1_ + impl_->dy_ + impl_->pos_.y);
}

void TreeView::i_doInput(int in)
{
	using namespace display::Keys;

	switch(in)
	{
		case 'w':
		case UP:
			impl_->up();
			break;
		case 's':
		case DOWN:
			impl_->down();
			break;
		case 'a':
		case LEFT:
			impl_->out();
			break;
		case 'd':
		case RIGHT:
			impl_->in();
			break;
		case ' ':
		case ENTER:
			impl_->cur()->toggle();
			impl_->updateView();
			break;
		case 'e':
			impl_->expand();
			break;
	}
}

// # ===========================================================================

namespace
{
	bool addPosIn(Node_ptr p, Node_ptr e, int& x, int& y)
	{
		if(p == e) return false;
		
		++y;

		if(!p->hasChildren() || !p->isOpen()) return true;

		++x;
		
		for(Node_ptr i : *p)
		{
			if(!addPosIn(i, e, x, y)) return false;
		}

		--x;

		return true;
	}
}

// # ---------------------------------------------------------------------------

void TreeView::Impl::recalculate(void)
{
	if(!needRecalculate_) return;

	pos_.x = 1;
	pos_.y = 0;

	addPosIn(root_, cur(), pos_.x, pos_.y);

	int w = x2_ - x1_, h = y2_ - y1_, mh = calcHeight();
	int x = x1_ + pos_.x * MXT_TABSTOP + 1;
	int y = y1_ + pos_.y;
	if(y + dy_ >= h)
	{
		dy_ = h - y - h / 4;
		if(dy_ < -mh + h) dy_ = -mh + h;
	}
	else if(y + dy_ < 0)
	{
		dy_ = -y + h / 4;
		if(dy_ > 0) dy_ = 0;
	}
	if(x + dx_ >= w)
	{
		dx_ = w - x - w / 4;
	}
	else if(x + dx_ < 0)
	{
		dx_ = -x + w / 4;
		if(dx_ > 0) dx_ = 0;
	}

	needRecalculate_ = false;
}

int TreeView::Impl::calcHeight(void) const
{
	std::function<int(Node_ptr)> calc = [&calc](Node_ptr p) -> int
		{
			int h = 1;
			if(p->hasChildren() && p->isOpen())
			{
				for(Node_ptr pp : *p)
				{
					h += calc(pp);
				}
			}
			return h;
		};
	
	return calc(root_);
}

void TreeView::Impl::updateView(void)
{
	ViewBuffer::vec_t vec;
	int x = 0, y = 0;

	std::function<void(Node_ptr)> update = [&update, &vec, &x, &y](Node_ptr node)
		{
			{
				std::string p;

				if(node->hasChildren())
				{
					p += node->isOpen() ? " -  " : " +  ";
				}
				else
				{
					p += " |  ";
				}

				p += node->getContent();

				ViewBuffer::line_t l;
				l.pos.x = x;
				l.pos.y = y;
				l.line  = p;

				vec.push_back(l);
			}

			if(node->isOpen() && node->hasChildren())
			{
				x += MXT_TABSTOP;
				for(const Node_ptr& p : *node)
				{
					++y; update(p);
				}
				x -= MXT_TABSTOP;
			}
		};
	
	update(root_);
	view_.update(std::move(vec));
}

const std::string& TreeView::Impl::getStatus(void) const
{
	using dav::aux::lexical_cast;

	if(status_.dirty)
	{
		std::stringstream ss;

		ss << obj_->filename() << " ";
		if(dirty_) ss << "[+] ";

		{
			size_t s = FileSystem::getFileSize(obj_->filename());
			const char *a = "BKMGT";
			int r = 0;
			while(s > 1024) { r = s % 1024; s /= 1024; ++a; }
			r = (50 + (1000 * r + 1023) / 1024) / 100;
			std::string pre = lexical_cast<std::string>(s);
			std::string post = r ? "." + lexical_cast<std::string>(r) : "";
			ss << pre << post << *a << " ";
		}

		ss  << (static_cast<bool>(current_) ? current_->index() : 0) 
			<< "," << (pos_.x - 1);

		status_.msg = ss.str();
		status_.dirty = false;
	}

	return status_.msg;
}

// # ---------------------------------------------------------------------------

void TreeView::Impl::up(void)
{
	if(current_)
	{
		current_->prev();
		needRecalculate_ = true;
	}
}

void TreeView::Impl::down(void)
{
	if(current_)
	{
		current_->next();
		needRecalculate_ = true;
	}
}

void TreeView::Impl::in(void)
{
	Node_ptr c(cur());
	if(!c->hasChildren()) return;
	if(!c->isOpen()) { c->open(); updateView(); }
	current_ = c;
	needRecalculate_ = true;
}

void TreeView::Impl::out(void)
{
	if(current_)
	{
		current_ = current_->parent();
		needRecalculate_ = true;
	}
}

void TreeView::Impl::expand(void)
{
	std::function<void(Node_ptr)> exp = [&exp](Node_ptr p)
		{
			if(p->hasChildren())
			{
				p->open();
				for(const auto& pp : *p)
				{
					exp(pp);
				}
			}
		};
	
	exp(cur());

	updateView();

	needRecalculate_ = true;
}

//void TreeView::Impl::erase(bool force)
//{
//	if(current_)
//	{
//		if(!force && current_->current()->hasChildren())
//		{
//			throw std::string("node has children. if you are sure, use '!' to override.");
//		}
//
//		current_->erase();
//		if(!current_->hasChildren()) out();
//		dirty();
//		updateView();
//		needRecalculate_ = true;
//	}
//	else
//	{
//		throw std::string("cannot delete root!");
//	}
//}
//
//void TreeView::Impl::insert(BYTE id, const std::string& name)
//{
//	if(id < 1 || id > 11) throw std::string("invalid id!");
//
//	Node_ptr p(cur());
//
//	nbt::NBT_ptr_t tag = nbt::Make(id, name);
//	nbt::NBT_ptr_t super = dynamic_cast<NBTNode *>(&*p)->getTag();
//
//	switch(super->getID())
//	{
//		case nbt::TAG_List::ID:
//		{
//			nbt::TAG_List::ptr_t list = std::dynamic_pointer_cast<nbt::TAG_List>(super);
//			if(list->tagID() && list->tagID() != id) throw std::string("invalid type for list!");
//			list->addTag(tag);
//			break;
//		}
//		case nbt::TAG_Compound::ID:
//		{
//			nbt::TAG_Compound::ptr_t nbt = std::dynamic_pointer_cast<nbt::TAG_Compound>(super);
//			nbt->setTag(tag);
//			break;
//		}
//		default:
//			throw std::string("only lists and compounds can have tags!");
//	}
//
//	NBTNode *node = new NBTNode;
//	Node_ptr pp(node);
//
//	node->load(tag);
//	
//	p->addChild(pp);
//
//	dirty();
//	updateView();
//}
//
//void TreeView::Impl::rename(const std::string& name)
//{
//	if(current_)
//	{
//		Node_ptr c(current_->current());
//		nbt::NBT_ptr_t tag = dynamic_cast<NBTNode *>(&*c)->getTag();
//		nbt::NBT_ptr_t parent = dynamic_cast<NBTNode *>(&*c->parent())->getTag();
//
//		if(tag->getName() == name) return;
//
//		if(parent->getID() == nbt::TAG_List::ID) throw std::string("list elements cannot be named!");
//
//		assert(parent->getID()==nbt::TAG_Compound::ID);
//
//		nbt::TAG_Compound::ptr_t nbt = std::dynamic_pointer_cast<nbt::TAG_Compound>(parent);
//
//		if(nbt->hasTag(name)) throw std::string("an element with that name already exists!");
//
//		tag->setName(name);
//
//		dirty();
//		updateView();
//	}
//	else
//	{
//		throw std::string("cannot name root!");
//	}
//}

// # ---------------------------------------------------------------------------

void TreeView::Impl::display(void) const
{
	view_.renderAt(x1_, y1_, x1_ - dx_, y1_ - dy_, x2_ - x1_, y2_ - y1_);
	Terminal::instance().setCursorPos(x1_, y2_);
	Terminal::instance().printf("%s", getStatus().data());
}

