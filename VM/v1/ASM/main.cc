#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>
#include "adef.h"
#include "Assembler.h"
#include "Reader.h"
#include "Logger.h"
#include "ASMException.h"

int main(int argc, char *argv[])
{
	using namespace vm;
	using namespace assembler;

	std::vector<std::string> args(argv, argv + argc);

	std::ifstream ins("ins.txt");

	if(!ins.is_open())
	{
		MXT_LOGANDTHROW("ERR: Couldn't open instructions!");
	}

	if(args.size() < 2)
	{
		MXT_LOGANDTHROW("ERR: Needs an filename argument!");
	}

	Assembler a(ins);

	a.assemble(args[1]);

	ins.close();

	std::ofstream out("out.bin", std::ios::out | std::ios::binary);
	a.out(out);
	out.close();

	LOG("THE END");

	return 0;
}

