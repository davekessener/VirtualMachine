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

	Builder makedea;

	makedea.addState("0");
	makedea.addState("1");
	makedea.addState("2");
	makedea.addState("3");
	makedea.addState("4");
	makedea.addState("5");
	makedea.addState("X");
	makedea.addSymbol("V");
	makedea.addSymbol("E");
	makedea.addSymbol("T");
	makedea.addSymbol("X");
	makedea.addSymbol("S");
	makedea.addRelation("0", "V", "1");
	makedea.addRelation("0", "E", "2");
	makedea.addRelation("0", "T", "3");
	makedea.addRelation("1", "E", "2");
	makedea.addRelation("2", "T", "3");
	makedea.addRelation("3", "X", "4");
	makedea.addRelation("3", "S", "5");
	makedea.addRelation("4", "S", "5");
	makedea.fillDelta("X");
	makedea.setInitialState("0");
	makedea.makeStateAccepting("5");

	auto dea = makedea.generate();

	dea = DEA::Read(std::cin);

	auto test = [&dea](const char *s)
	{
		std::string b(s);
		std::vector<std::string> v;

		while(*s)
		{
			std::string t;
			t.push_back(*s);
			v.emplace_back(std::move(t));
			++s;
		}

		std::cout << "'" << b << "' " << (dea.accepts(v.cbegin(), v.cend()) ? "T" : "F") << std::endl;
	};

	auto tbl = dea.generateTable();

	for(const auto& e : tbl)
	{
		if(!e.second) std::cout << e.first.first << ", " << e.first.second << std::endl;
	}

	return 0;
}
catch(const std::string& e)
{
	std::cerr << e << std::endl;
}

