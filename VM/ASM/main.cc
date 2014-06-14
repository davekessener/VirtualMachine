#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>
#include "Assembler.h"
#include "Reader.h"

int main(int argc, char *argv[])
{
	using namespace vm;
	using namespace assembler;

	std::vector<std::string> args(argv, argv + argc);

	std::ifstream in("test.s");

	assert(in.is_open());

	Reader r(in);

	Assembler a;

	a.assemble(r);

	in.close();

	return 0;
}

