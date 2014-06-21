#include <iostream>
#include <fstream>
#include <cassert>

int main(void)
{
	std::ifstream in("test.s");

	assert(in.is_open());

	while(!in.eof())
	{
		std::string s;
		std::getline(in, s);
		std::cout << s << '\n';
	}

	in.close();

	std::cout << std::flush;

	return 0;
}

