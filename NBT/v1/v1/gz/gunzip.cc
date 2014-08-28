#include "gzstream.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if(argc != 3)
	{
		std::cerr << "Usage: " << argv[0] << " <in-file> <out-file>\n";
		return EXIT_FAILURE;
    }

    igzstream in(argv[1]);

    if(!in.good())
	{
        std::cerr << "ERROR: Opening file `" << argv[1] << "' failed.\n";

		return EXIT_FAILURE;
    }

    std::ofstream out(argv[2]);

    if(!out.good())
	{
        std::cerr << "ERROR: Opening file `" << argv[2] << "' failed.\n";

		return EXIT_FAILURE;
    }

    char c;
    while(in.get(c))
	{
		out << c;
	}

    in.close();
    out.close();
    
	if(!in.eof())
	{
        std::cerr << "ERROR: Reading file `" << argv[1] << "' failed.\n";
		
		return EXIT_FAILURE;
    }

    if(!out.good())
	{
        std::cerr << "ERROR: Writing file `" << argv[2] << "' failed.\n";
		
		return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

