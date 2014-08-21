#include <sstream>
#include <stack>
#include <functional>
#include "TreeView.h"
#include <dav/Logger.h>
#include <aux>
#include "Terminal.h"
#include "Node.h"
#include "FileSystem.h"
#include "ViewBuffer.h"

#define MXT_TABSTOP 4

using display::Terminal;

// # ---------------------------------------------------------------------------

struct TreeView::Impl
{
	void display( ) const;
	void display(Node_ptr, int, int&) const;
	void initPos( );
	void up( );
	void down( );
	void in( );
	void out( );
	void expand( );
	void erase( );
	Node_ptr cur( ) { return static_cast<bool>(current_) ? current_->current() : root_; }
	void recalculate( );
	int calcHeight( ) const;
	void updateView( );
	void dirty(bool f = true) { dirty_ = f; }
	std::string getStatusMsg( ) const;

	int x1_, y1_, x2_, y2_;
	int dx_, dy_;
	Object_ptr obj_;
	Node_ptr root_, current_;
	struct { int x, y; } pos_;
	bool dirty_;
	std::string status_;
	ViewBuffer view_;
};

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

	assert(x>=0&&y>=0&&w>0&&h>0);

	impl_->initPos();
}

TreeView::~TreeView(void) noexcept
{
	delete impl_;
}

bool TreeView::isModified(void) const
{
	return impl_->dirty_;
}

void TreeView::modify(bool v)
{
	impl_->dirty(v);
}

// # ---------------------------------------------------------------------------

void TreeView::i_doRender(void) const
{
	if(!static_cast<bool>(impl_->root_)) return;

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
		case 'x':
			impl_->erase();
			break;
	}

	impl_->recalculate();
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

	status_ = getStatusMsg();
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

std::string TreeView::Impl::getStatusMsg(void) const
{
	using lib::aux::lexical_cast;

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

	return ss.str();
}

// # ---------------------------------------------------------------------------

void TreeView::Impl::initPos(void)
{
	current_.reset();
	pos_.x = 1;
	pos_.y = 0;
	status_ = getStatusMsg();
	updateView();
}

void TreeView::Impl::up(void)
{
	if(current_)
	{
		current_->prev();
	}
}

void TreeView::Impl::down(void)
{
	if(current_)
	{
		current_->next();
	}
}

void TreeView::Impl::in(void)
{
	Node_ptr c(cur());
	if(!c->hasChildren()) return;
	if(!c->isOpen()) { c->open(); updateView(); }
	current_ = c;
}

void TreeView::Impl::out(void)
{
	if(current_)
	{
		current_ = current_->parent();
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
}

void TreeView::Impl::erase(void)
{
	if(current_)
	{
		current_->erase();
		if(!current_->hasChildren()) out();
		dirty();
		updateView();
	}
}

// # ---------------------------------------------------------------------------

void TreeView::Impl::display(void) const
{
//	int y(y1_ + dy_); 
//	display(root_, x1_ + dx_, y);
	view_.renderAt(x1_, y1_, x1_ - dx_, y1_ - dy_, x2_ - x1_, y2_ - y1_);
	Terminal::instance().setCursorPos(x1_, y2_);
	Terminal::instance().printf("%s", status_.data());
}

void TreeView::Impl::display(Node_ptr node, int x, int& y) const
{
	if(x >= x2_ || y >= y2_) return;

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

	if((long)p.length() > x2_ - x) std::string(p.data(), x2_ - x).swap(p);

	if(x >= x1_ && y >= y1_)
	{
		Terminal::instance().setCursorPos(x, y);
		Terminal::instance().printf("%s", p.data());
	}

	if(node->isOpen() && node->hasChildren())
	{
		for(const Node_ptr& p : *node)
		{
			display(p, x + 4, ++y);
		}
	}
}

