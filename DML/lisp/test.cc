#include <iostream>
#include "Numeric.hpp"

using namespace lisp::numeral;

template<typename T>
struct Print : public Print<typename T::TAIL>
{
	void print(void)
	{
		std::cout << T::HEAD << std::endl;

		Print<typename T::TAIL>::print();
	}
};

template<>
struct Print<NIL>
{
	virtual void print(void)
	{
	}
};

int main(int argc, char *argv[])
{
	typedef BL<1, BL<2, BL<3, BL<42, NIL>>>> S;

	Print<S> p1;
	Print<typename ReverseBL<S>::RET> p2;

	p1.print();
	p2.print();

	return 0;
}

