#include <iostream>
#include <functional>
#include <cstdarg>

struct A
{
	void operator()(const std::string& args)
	{
		std::cout << "Arguments: '" << args << "'." << std::endl;
	}
};

int main(void)
{
	A a;
	std::function<void(const std::string&)> f = a;

	f("7 42");

	return 0;
}

