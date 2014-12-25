#include <iostream>
#include <vector>
#include "HuffTree.hpp"

int main(int argc, char *argv[])
try
{
	std::vector<std::string> args(argv + 1, argv + argc);
	std::string s("");

	{bool f(true); for(const auto& v : args) { if(!f) s += " "; s += v; f = false; } }

	Node_ptr root = Node::CreateTree(s.cbegin(), s.cend());
	std::string encoded(root->encode(s.cbegin(), s.cend()));
	std::string decoded(root->decode(encoded.cbegin(), encoded.cend()));

	std::cout << root->toString() << std::endl;
	std::cout << encoded << std::endl << decoded << std::endl;

	return 0;
}
catch(const std::string& e)
{
	std::cerr << e << std::endl;
}

