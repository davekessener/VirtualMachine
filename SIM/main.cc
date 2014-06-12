#include <iostream>
#include "Simulator.h"

int main(int argc, char *argv[])
{
	std::string fn;
	int t = 1;

	if(argc > 1)
	{
		fn = argv[1];
	}
	else
	{
		std::cout << "chip-name: ";
		std::cin >> fn;
	}

	if(argc > 2)
	{
		if(argv[2][0] >= '0' && argv[2][0] <= '9')
		{
			t = std::atoi(argv[2]);
		}
	}

	sim::Simulator s(fn);

	s.run(t);

	return 0;
}

