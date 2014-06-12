#include <iostream>
#include <iomanip>
#include <vector>
#include "BigUInt.h"

int main(int argc, char *argv[])
{
	using namespace lisp;

	std::vector<std::string> args(argv, argv + argc);

	BigUInt v(0xfffffffffffff);
	v.add(v);
//	BigUInt a(v);
	
	std::cout << "Print: " << std::endl;
	std::cout << v << std::endl;
//	for(int i = 0 ; i < 10 ; ++i)
//	a.add(v);
//	std::cout << a << std::endl;

	return 0;
}

