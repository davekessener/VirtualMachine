#include <stack>
#include "TreeView.h"
#include "Terminal.h"
#include <dav/Logger.h>

#define MXT_TABSTOP 4

using display::Terminal;

struct TreeView::Impl
{
	typedef Node::iterator iter;
	struct Branch
	{
		inline Branch(Node_ptr p) : node(p), i(p->begin()) { }
		inline Branch(Node_ptr p, iter j) : node(p), i(j) { }

		Node_ptr node;
		iter i;
	};
	typedef std::stack<Branch> nstack_t;

	void display( ) const { int y(y1_); display(root_, x1_, y); }
	void display(Node_ptr, int, int&) const;
	void initPos( );
	void up( );
	void down( );
	void erase( );
	Node_ptr cur( ) { return cur_.nodes.empty() ? root_ : *cur_.nodes.top().i; }
	inline bool isEnd(Branch& b) { iter i(b.i); return ++i == b.node->end(); }

	int x1_, y1_, x2_, y2_;
	Node_ptr root_;
	struct
	{
		nstack_t nodes;
		int y, x;
	} cur_;
};

TreeView::TreeView(int x, int y, int w, int h) : impl_(new Impl)
{
	impl_->x1_ = x;
	impl_->y1_ = y;
	impl_->x2_ = x + w;
	impl_->y2_ = y + h;

	assert(x>=0&&y>=0&&w>0&&h>0);
}

TreeView::~TreeView(void) noexcept
{
	delete impl_;
}

void TreeView::setRoot(Node_ptr p)
{
	impl_->root_ = p;
	impl_->initPos();
}

void TreeView::render(void) const
{
	if(!static_cast<bool>(impl_->root_)) return;

	impl_->display();

	Terminal::instance().setCursorPos(impl_->cur_.x, impl_->cur_.y);
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
			impl_->cur()->close();
			break;
		case 'd':
			impl_->cur()->open();
			break;
		case '\n':
			impl_->cur()->toggle();
			break;
		case 'x':
			impl_->erase();
			break;
	}
}

void TreeView::Impl::initPos(void)
{
	nstack_t().swap(cur_.nodes);

	cur_.x = x1_ + 1;
	cur_.y = y1_;
}

void TreeView::Impl::up(void)
{
	if(cur_.nodes.empty()) return;

	Branch cur(cur_.nodes.top());

	if(cur.i == cur.node->begin())
	{
		cur_.nodes.pop();
		--cur_.y;
		cur_.x -= MXT_TABSTOP;
	}
	else
	{
		--cur_.nodes.top().i;
		--cur_.y;

		cur = cur_.nodes.top();
		if((*cur.i)->hasChildren() && (*cur.i)->isOpen())
		{
			cur_.nodes.push(Branch(*cur.i, (*cur.i)->end()));
			--cur_.nodes.top().i;
			cur_.x += MXT_TABSTOP;
		}
	}
}

void TreeView::Impl::down(void)
{
	if(cur_.nodes.empty())
	{
		if(root_->hasChildren() && root_->isOpen())
		{
			cur_.nodes.push(Branch(root_));
			++cur_.y;
			cur_.x += MXT_TABSTOP;
		}

		return;
	}

	Branch cur(cur_.nodes.top());

	if((*cur.i)->hasChildren() && (*cur.i)->isOpen())
	{
		cur_.nodes.push(Branch(*cur.i));
		++cur_.y;
		cur_.x += MXT_TABSTOP;
	}
	else if(isEnd(cur))
	{
		cur_.nodes.pop();

		if(!cur_.nodes.empty() && !isEnd(cur_.nodes.top()))
		{
			++cur_.nodes.top().i;
			++cur_.y;
			cur_.x -= MXT_TABSTOP;
		}
		else
		{
			cur_.nodes.push(cur);
		}
	}
	else
	{
		++cur_.nodes.top().i;
		++cur_.y;
	}
}

void TreeView::Impl::erase(void)
{
	Branch &cur(cur_.nodes.top());
	cur.node.eras
}

void TreeView::Impl::display(Node_ptr node, int x, int& y) const
{
	if(x >= x2_ || y >= y2_) return;

	std::string p;

	if(node->hasChildren())
	{
		p += node->isOpen() ? "[-] " : "[+] ";
	}
	else
	{
		p += "    ";
	}

	p += node->getContent();

	if((long)p.length() > x2_ - x) std::string(p.data(), x2_ - x).swap(p);

	Terminal::instance().setCursorPos(x, y);
	Terminal::instance().printf("%s", p.data());

	if(node->isOpen() && node->hasChildren())
	{
		for(const Node_ptr& p : *node)
		{
			display(p, x + 4, ++y);
		}
	}
}

