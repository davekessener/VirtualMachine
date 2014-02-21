#ifndef TREEVIEW_H
#define TREEVIEW_H

#include "Node.h"

#ifdef TREEVIEW_MAIN
#endif

class TreeView
{
	typedef Node::Node_ptr_t Node_ptr_t;

	public:
		TreeView(Node_ptr_t);
		virtual inline ~TreeView( ) { }
		std::vector<std::pair<int, std::string>> getTreeDisplay( );
		bool isOpen(const std::string& path) { return getNode(path)->isOpen(); }
		bool hasChildren(const std::string& path) { return getNode(path)->hasChildren(); }
	protected:
		Node_ptr_t getNode(const std::string&);
		void addData(std::vector<std::pair<int, std::string>>&, Node_ptr_t, int);
	private:
		std::vector<std::string> splitPath(const std::string&);
		Node_ptr_t root;
};

#endif

