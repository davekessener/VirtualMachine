#include <iostream>
#include "Tokenizer.h"

int main(int argc, char *argv[])
try
{
	dav::io::OStreamContainer<std::string> out(std::ostream_iterator<std::string>(std::cout, "\n"));

	while(1)
	{
		std::string s("");

		std::getline(std::cin, s);

		if(std::cin.eof()) break;

		if(s.empty()) continue;

		dav::tokenizer::parse(s.cbegin(), s.cend(), out);
	}

	return 0;
}
catch(const std::string& e)
{
	std::cerr << std::endl << "ERR: " << e << std::endl;
	return 1;
}

