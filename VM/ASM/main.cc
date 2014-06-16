#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>
#include "Assembler.h"
#include "Reader.h"
#include "Logger.h"

int main(int argc, char *argv[])
{
	using namespace vm;
	using namespace assembler;

	std::vector<std::string> args(argv, argv + argc);

	std::ifstream ins("ins.txt");

	assert(ins.is_open());

	Assembler a(ins);

	a.assemble("test.s");

	ins.close();

	std::ofstream out("tst.bin", std::ios::out | std::ios::binary);
	a.out(out);
	out.close();

	LOG("THE END");

	return 0;
}

