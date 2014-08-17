#include <string>
#include "TreeView.h"

TreeView::TreeView(Node_ptr_t _root) : root(_root)
{
}

std::vector<std::pair<int, std::string>> TreeView::getTreeDisplay(void)
{
	std::vector<std::pair<int, std::string>> v;

	addData(v, root, 0);

	return v;
}

// # ---------------------------------------------------------------------------

TreeView::Node_ptr_t TreeView::getNode(const std::string& path)
{
	std::vector<std::string> v = splitPath(path);
	Node_ptr_t node = root;

	for(const std::string& s : v)
	{
		if(!node->hasChildren()) return Node_ptr_t(NULL);
		std::vector<Node_ptr_t> c = node->getChildren();

		for(auto i = c.begin() ; ; )
		{
			if((*i)->getValue() == s)
			{
				node = *i;
				break;
			}

			if(++i == c.end()) return Node_ptr_t(NULL);
		}
	}

	return node;
}

void TreeView::addData(std::vector<std::pair<int, std::string>>& v, Node_ptr_t node, int indent)
{
	v.push_back(std::make_pair(indent++, node->getValue()));

	if(node->isOpen())
	{
		for(Node_ptr_t p : node->getChildren())
		{
			addData(v, p, indent);
		}
	}
}

// # ---------------------------------------------------------------------------

std::vector<std::string> TreeView::splitPath(const std::string& path)
{
	std::vector<std::string> v;

	std::string::const_iterator i = path.cbegin(), j = path.cbegin();
	while(i != path.cend())
	{
		if(*i != '/') { ++i; continue; }
		
		if(i != j)
		{
			v.push_back(std::string(j, i));
		}

		j = ++i;
	}

	if(i != j) v.push_back(std::string(j, i));

	return v;
}

