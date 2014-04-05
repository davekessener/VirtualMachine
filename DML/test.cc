#include <iostream>
#include "DML.h"
#include "TypeList.hpp"

template<typename T>
struct Print
{
	typedef typename dml::typelist::Transfer<T, std::pair>::RET TT;

	Print(TT& t) : _v(t) { }

	void print( )
	{
		std::cout << _v.first << std::endl;

		Print<typename T::TAIL> p(_v.second);

		p.print();
	}

	TT& _v;
};

template<typename T>
struct Print<dml::TypeList<T, NULL_t>>
{
	Print(T& t) : _v(t) { }

	void print( )
	{
		std::cout << _v << std::endl;
	}

	T& _v;
};

template<typename ... T>
struct Tuple
{
	typedef typename dml::typelist::MakeTypeList<T...>::RET typelist;
	typedef typename dml::typelist::Transfer<typelist, std::pair>::RET type;

	void print( )
	{
		Print<typelist> p(_v);

		p.print();
	}

	type _v;
};

int main(int argc, char *argv[])
{
	auto a = new dml::typelist::MakeTypeList<int, long, short, char>::RET;
	auto b = new Tuple<double, int, char, short>;

	b->_v.first = 3.141;
	b->_v.second.first = 1;
	b->_v.second.second.first = 'Z';
	b->_v.second.second.second = -1;

	b->print();

	std::cout << "---" << std::endl;

	std::cout << dml::typelist::Find<decltype(*a), short>::VAL << std::endl;

	std::cout << sizeof(*b) << std::endl;

	delete a;
	delete b;

	return 0;
}

