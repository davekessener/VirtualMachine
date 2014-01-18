#include <iostream>
#include <stdlib.h>
#include "A.hpp"
#include "pVector.h"

int main()
{
	typedef A<7> A_7;

	std::cout << std::endl << std::endl << std::endl;

#define NL std::cout<<std::endl;
	pVector<A_7> v;

	std::cout << v.toString() << std::endl;

	NL;
	for(int i = 0 ; i < 57 ; i++)
	{
		v.push(new A_7((rand() % 10000) / 10000.0));
	}
	NL;

	std::cout << v.toString() << std::endl;

	delete v.poll();

	std::cout << v.toString() << std::endl;

	delete v.pop();

	std::cout << v.toString() << std::endl;

	NL;
	for(int i = 0 ; i < 48 ; i++)
	{
		delete v.poll();
	}
	NL;

	std::cout << v.toString() << std::endl;

	while(v.Length() > 0)
	{
		delete v.poll();
	}

	NL;
	std::cout << v.toString() << std::endl;
	NL;

	return 0;
}

