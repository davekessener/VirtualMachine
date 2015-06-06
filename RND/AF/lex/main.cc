#include <iostream>
#include <vector>
#include <string>
#include <iterator>
#include "DEA.h"

int main(int argc, char *argv[])
try
{
	std::vector<std::string> args(argv + 1, argv + argc);

	using namespace AF::DEA;

	auto dea = DEA::Read(std::cin);

	auto tbl = dea.generateTable();

	std::vector<std::set<State>> eqs;
	for(const auto& e : tbl)
	{
		if(!e.second)
		{
			bool d = false;
			for(auto& o : eqs)
			{
				if(o.find(e.first.first) != o.end())
				{
					o.insert(e.first.second);
					d = true;
					break;
				}
				else if(o.find(e.first.second) != o.end())
				{
					o.insert(e.first.first);
					d = true;
					break;
				}
			}
			if(!d)
			{
				std::set<State> t;
				t.insert(e.first.first);
				t.insert(e.first.second);
				eqs.emplace_back(std::move(t));
			}
		}
	}

	for(const auto& eq : eqs)
	{
		bool f = true;
		for(const auto& e : eq)
		{
			if(!f) std::cout << ", ";
			std::cout << e;
			f = false;
		}
		std::cout << std::endl;
	}

	return 0;
}
catch(const std::string& e)
{
	std::cerr << e << std::endl;
}

