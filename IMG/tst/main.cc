#include <iostream>
#include "A.hpp"
#include "B.hpp"

int main()
{
	A<double> a(3.1415);
	B<double> b(a);

	a.print();
	b.print();

	return 0;
}

