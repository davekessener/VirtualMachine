#include <iostream>
#include "Simulator.h"

int main(int argc, char *argv[])
{
	std::cout << "chip-name: ";
	std::string fn;
	std::cin >> fn;

	sim::Simulator s(fn);

	s.run();

	return 0;
}

