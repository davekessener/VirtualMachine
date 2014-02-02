#include <iostream>
#include "TypeList.h"

int main(void)
{
	typedef typename TL::Construct<int, double, short>::list ListOne;
	typedef typename TL::Construct<char, float>::list ListTwo;
	typedef typename TL::Construct<int, char, long>::list ListThree;
	typedef typename TL::Union<ListOne, ListTwo>::list UnionList;

	std::cout << "Length: ";
	std::cout << TL::Length<UnionList>::length;
	std::cout << std::endl;

	return 0;
}

