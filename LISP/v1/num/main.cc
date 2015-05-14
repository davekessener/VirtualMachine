#include <iostream>
#include <iomanip>
#include <vector>
#include <limits>
#include "BigUInt.h"
#include "limits.hpp"

int main(int argc, char *argv[])
{
	using namespace lisp;

	std::vector<std::string> args(argv, argv + argc);

	BigUInt v1(args.at(1));
	BigUInt v2(args.at(2));
	BigUInt v(v1);
	
	std::cout << "---" << std::endl;
	v.mul(v2);
	std::cout << "---" << std::endl;
	std::cout << v1 << " * " << v2 << " == " << std::flush;
	std::cout << v << std::endl;
	std::cout << "---" << std::endl;
	std::cout << BigUInt(std::numeric_limits<unsigned long long>::max()) << std::endl;

	return 0;
}

