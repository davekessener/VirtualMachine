#include <iostream>
#include <vector>
#include "Poly.h"

int main(int argc, char *argv[])
{
	std::vector<std::string> args(argv, argv + argc);

	if(args.size() < 3)
	{
		std::cout << "Usage: " << args.at(0) << " \"numerator\" \"demoninator\"" << std::endl;
		return 0;
	}

	Poly n(args.at(1)), d(args.at(2));
	std::vector<Poly> r(n / d);

	std::cout << n << std::endl << d << std::endl;

	for(auto i = r.cbegin() ; i != r.cend() ; ++i)
	{
		if(i != r.cbegin()) std::cout << " * ";
		std::cout << "(" << *i << ")";
	}

	std::cout << std::endl;

	return 0;
}

