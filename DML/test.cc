#include <iostream>
#include <cassert>
#include "DML.h"
#include "TypeList.hpp"

template<typename T>
struct Print
{
	typedef typename dml::typelist::Transfer<T, std::pair>::RET TT;

	constexpr Print(const TT& t) : _v(t) { }

	constexpr void print( )
	{
		std::cout << _v.first << std::endl;

		Print<typename T::TAIL> p(_v.second);

		p.print();
	}

	const TT& _v;
};

template<typename T>
struct Print<dml::TypeList<T, NULL_t>>
{
	constexpr Print(const T& t) : _v(t) { }

	constexpr void print( )
	{
		std::cout << _v << std::endl;
	}

	const T& _v;
};

template<typename ... T>
struct Tuple
{
	typedef typename dml::typelist::MakeTypeList<T...>::RET typelist;
	typedef typename dml::typelist::Transfer<typelist, std::pair>::RET type;

	constexpr void print( )
	{
		Print<typelist> p(_v);

		p.print();
	}

	type _v;
};

int main(int argc, char *argv[])
{
	typedef typename dml::typelist::MakeTypeList<int, long, float, short, char>::RET list;
	Tuple<double, int, char, short> b;

	dml::typelist::Get<list, dml::typelist::Find<list, char>::VAL>::RET v;

	v = 'a';

	b._v.first = 3.141;
	b._v.second.first = 1;
	b._v.second.second.first = 'Z';
	b._v.second.second.second = -1;

	b.print();

	std::cout << "---" << std::endl;

	std::cout << v << std::endl;

	std::cout << dml::typelist::Length<list>::VAL << std::endl;

	std::cout << dml::typelist::Find<list, short>::VAL << std::endl;

	std::cout << sizeof(b) << std::endl;

	return 0;
}

