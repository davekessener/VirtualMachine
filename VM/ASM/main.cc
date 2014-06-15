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

	std::ifstream ins("ins.txt");
	std::ifstream in("test.s");

	assert(in.is_open());
	assert(ins.is_open());

	Assembler a(ins);

	a.assemble(in);

	in.close();

	a.out(std::cout);

	return 0;
}

