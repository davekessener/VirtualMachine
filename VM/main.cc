#include <iostream>
#include <vector>
#include "ddef.h"
#include "CPU.h"
#include "Instruction.h"
#include "Processor.h"

int main(int argc, char *argv[])
{
	using namespace vm;
	using namespace cpu;

	std::vector<std::string> args(argv, argv + argc);

	Processor p;
	WORD code[] = {0x01, 0x0c, 0x1234, 0x00};

	p.reset();
	std::cout << p << std::endl;
	p.write(0, 4, code);
	p.run();
	std::cout << p << std::endl;

	std::cout << p.printOpcodes() << std::endl;

	return 0;
}

