#ifndef NBTEDIT_TREEVIEW_H
#define NBTEDIT_TREEVIEW_H

#include "Object.h"

class TreeView
{
	public:
		TreeView(int, int, int, int, Object_ptr);
		~TreeView( ) noexcept;
		void render( ) const;
		void input(int);
	private:
		struct Impl;
		Impl *impl_;
};

#endif

