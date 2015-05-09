#include <iostream>
#include <vector>
#include "CPU.h"

int main(int argc, char *argv[])
{
	std::vector<std::string> args(argv, argv + argc);

	using namespace vm;

	BYTE boot[] = { 0x01, 0x00, 0x05, 0x00, 0x07, 0x00, 0x0A, 0x00, 0x00};

	set_ram(0, boot, sizeof(boot));

	run();

	print_reg(std::cout);

	return 0;
}

