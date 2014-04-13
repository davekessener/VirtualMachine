#include <iostream>
#include <cassert>
#include "DML.h"
#include "TypeList.hpp"
#include "Integral.hpp"
#include "Logic.hpp"

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
	typename dml::If<dml::Bool<false>, int, double>::RET val;

	val = static_cast<decltype(val)>(3.1);

	std::cout << val << std::endl;

	return 0;
}

