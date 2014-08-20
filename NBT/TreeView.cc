#include <stack>
#include <functional>
#include "TreeView.h"
#include "Terminal.h"
#include "Node.h"
#include <dav/Logger.h>

#define MXT_TABSTOP 4

using display::Terminal;

struct TreeView::Impl
{
	void display( ) const { int y(y1_ + dy_); display(root_, x1_ + dx_, y); }
	void display(Node_ptr, int, int&) const;
	void initPos( );
	void up( );
	void down( );
	void in( );
	void out( );
	void erase( );
	Node_ptr cur( ) { return static_cast<bool>(current_) ? current_->current() : root_; }
	void recalculate( );
	int calcHeight( ) const;

	int x1_, y1_, x2_, y2_;
	int dx_, dy_;
	Object_ptr obj_;
	Node_ptr root_, current_;
	struct { int x, y; } pos_;
};

TreeView::TreeView(int x, int y, int w, int h, Object_ptr o) : impl_(new Impl)
{
	impl_->x1_ = x;
	impl_->y1_ = y;
	impl_->x2_ = x + w;
	impl_->y2_ = y + h;
	impl_->dx_ = 0;
	impl_->dy_ = 0;
	impl_->obj_ = o;
	impl_->root_ = o->get();

	assert(x>=0&&y>=0&&w>0&&h>0);

	impl_->initPos();
}

TreeView::~TreeView(void) noexcept
{
	delete impl_;
}

void TreeView::render(void) const
{
	if(!static_cast<bool>(impl_->root_)) return;

	impl_->display();

	Terminal::instance().setCursorPos(
		impl_->x1_ + impl_->dx_ + impl_->pos_.x * MXT_TABSTOP + 1,
		impl_->y1_ + impl_->dy_ + impl_->pos_.y);
}

void TreeView::input(int in)
{
	switch(in)
	{
		case 'w':
			impl_->up();
			break;
		case 's':
			impl_->down();
			break;
		case 'a':
			impl_->out();
			break;
		case 'd':
			impl_->in();
			break;
		case ' ':
		case '\n':
			impl_->cur()->toggle();
			break;
		case 'x':
			impl_->erase();
			break;
	}

	impl_->recalculate();
}

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

void TreeView::Impl::recalculate(void)
{
	pos_.x = 1;
	pos_.y = 0;

	addPosIn(root_, cur(), pos_.x, pos_.y);

	int w = x2_ - x1_, h = y2_ - y1_, mh = calcHeight();
	if(pos_.y + dy_ >= h)
	{
		dy_ = h - pos_.y - h / 4;
		if(dy_ < -mh + h) dy_ = -mh + h;
	}
	else if(pos_.y + dy_ < 0)
	{
		dy_ = -pos_.y + h / 4;
		if(dy_ > 0) dy_ = 0;
	}
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

void TreeView::Impl::initPos(void)
{
	current_.reset();
	pos_.x = 1;
	pos_.y = 0;
}

void TreeView::Impl::up(void)
{
	if(current_)
	{
		current_->prev();
	}
}

void TreeView::Impl::out(void)
{
	if(current_)
	{
		current_ = current_->parent();
	}
}

void TreeView::Impl::in(void)
{
	Node_ptr c(cur());
	if(!c->hasChildren()) return;
	if(!c->isOpen()) c->open();
	current_ = c;
}

void TreeView::Impl::down(void)
{
	if(current_)
	{
		current_->next();
	}
}

void TreeView::Impl::erase(void)
{
	if(current_)
	{
		current_->erase();
		if(!current_->hasChildren()) out();
	}
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

