#include <iostream>
#include <vector>
#include <functional>
#include "BST.hpp"

int main(int argc, char *argv[])
try
{
	std::vector<std::string> args(argv, argv + argc);

	using namespace dav;

	BinarySearchTree<int, std::string, std::hash<int>> bst;

	bst.put(3, std::string("hello"));
	bst.put(1, std::string("world"));
	bst.put(8, std::string("hello"));
	bst.put(4, std::string("there"));
	bst.put(0, std::string("handsome"));
	std::cout << bst.get(1) << std::endl;

	std::cout << (bst.contains(0) ? "yes" : "no") << " and " << (bst.contains(7) ? "yes" : "no") << std::endl;

	return 0;
}
catch(const std::string& e)
{
	std::cerr << e << std::endl;
}

