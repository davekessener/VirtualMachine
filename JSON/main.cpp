#include <iostream>
#include <vector>

int main(int argc, char *argv[])
try
{
	std::vector<std::string> args(argv + 1, argv + argc);

	return 0;
}
catch(const std::string& e)
{
	std::cerr << e << std::endl;
}

