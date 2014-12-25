#include <iostream>
#include <map>
#include <set>
#include <vector>
#include "CountElems.hpp"

int main(int argc, char *argv[])
try
{
	std::vector<std::string> args(argv + 1, argv + argc);

	std::map<char, int> map;
	std::set<long> fin;

	countElements(args.at(0).cbegin(), args.at(0).cend(), map);

	for(const auto& p : map)
	{
		fin.insert((p.second << 8) | p.first);
	}

	for(const auto& v : fin)
	{
		std::cout << (char)(v & 0xff) << ": " << (v >> 8) << std::endl;
	}

	return 0;
}
catch(const std::string& e)
{
	std::cerr << e << std::endl;
}

