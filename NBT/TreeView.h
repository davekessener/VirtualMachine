#ifndef NBTEDIT_TREEVIEW_H
#define NBTEDIT_TREEVIEW_H

#include "Node.h"

class TreeView
{
	public:
		TreeView(int, int, int, int);
		~TreeView( ) noexcept;
		void setRoot(Node_ptr);
		void render( ) const;
	private:
		struct Impl;
		Impl *impl_;
};

#endif

