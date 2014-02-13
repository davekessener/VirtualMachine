#include <iostream>
#include <functional>

namespace A
{
	enum
	{
		B,
		C,
		D
	};
}

int tst(void) { return A::D; }

int main(void)
{
	std::cout << tst() << std::endl;

	return 0;
}

