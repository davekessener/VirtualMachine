#include <iostream>
#include "DML.h"
#include "TypeList.hpp"

template<typename T, typename TT>
void printTuple(std::pair<T, TT>);

template<typename ... T>
struct Tuple
{
	typedef typename dml::typelist::Transfer<typename dml::typelist::MakeTypeList<T...>::RET, std::pair>::RET type;

	void print( )
	{
		printTuple(_v);
	}

	type _v;
};

template<typename H, typename T>
void printTuple(std::pair<H, T> p)
{
	std::cout << p.first << std::endl;

	printTuple(p.second);
}

int main(int argc, char *argv[])
{
	auto a = new dml::typelist::MakeTypeList<int, long, short, char>::RET;
	auto b = new Tuple<long, int, char, short>;

	b->_v.first = 0;
	b->_v.second.first = 1;
	b->_v.second.second.first = 2;
//	b->_v.second.second.second.first = 3;

	b->print();

	std::cout << "---" << std::endl;

	std::cout << dml::typelist::Find<decltype(*a), short>::VAL << std::endl;

	std::cout << sizeof(*b) << std::endl;

	delete a;
	delete b;

	return 0;
}

