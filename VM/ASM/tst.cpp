#include <iostream>
#include <sstream>

int main(void)
{
	std::string s("0x00: hlt    ");

	std::istringstream iss(s);

	while(iss)
	{
		char c = 0;
		iss >> c;
		std::cout << "'" << c << "'(" << static_cast<int>(c) << ")" << std::endl;
	}

	return 0;
}

