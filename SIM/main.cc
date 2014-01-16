#include <iostream>
#include "Simulator.h"

#define EXIT_FAILURE 1

using namespace std;

int main(int argc, char *argv[])
{
	if(argc < 2)
	{
		cerr << "ERR: Usage: " << argv[0] << " simfile.dsp" << endl;
		return EXIT_FAILURE;
	}

	return Simulator::simulate(argv[1]);
}

