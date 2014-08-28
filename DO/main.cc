#include <iostream>
#include <vector>
#include "Scan.h"

int main(int argc, char *argv[])
{
	std::vector<std::string> args(argv, argv + argc);

	return Scan::run(args);
}

