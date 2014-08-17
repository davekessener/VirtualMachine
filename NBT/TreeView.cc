#include "TreeView.h"

struct TreeView::Impl
{
};

TreeView::TreeView(int x, int y, int w, int h) : impl_(new Impl)
{
}

TreeView::~TreeView(void) noexcept
{
	delete impl_;
}

void TreeView::setRoot(Node_ptr p)
{
}

void TreeView::render(void) const
{
}

