#include <iostream>
#include <functional>
#include <cstdarg>

struct A
{
	bool tst(int i)
	{
		std::cout << "INT: " << i << std::endl;
		return false;
	}

	std::function<bool(int)> getFn(void)
	{
		return std::bind(&A::tst, this, std::placeholders::_1);
	}
};

int main(void)
{
	A a;
	std::function<bool(int)> fn = a.getFn();

	fn(42);

	return 0;
}

