#ifndef NBTEDIT_TREEVIEW_H
#define NBTEDIT_TREEVIEW_H

#include "Control.h"
#include "Object.h"

class TreeView : public Control
{
	public:
		TreeView(int, int, int, int, Object_ptr);
		~TreeView( ) noexcept;
		bool isModified( ) const;
		void modify(bool = true);
	private:
		void i_doRender( ) const;
		void i_doInput(int);
	private:
		struct Impl;
		Impl *impl_;
};

#endif

