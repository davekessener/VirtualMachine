#include <iostream>
#include "Tokenizer.h"
#include "InfToPost.h"
#include "Evaluator.h"

int main(int argc, char *argv[])
try
{
	std::vector<std::string> args(argv + 1, argv + argc);

	std::vector<std::string> tokens, postbuf;
	std::string in;

	{
		std::ostringstream oss;
		for(std::istream_iterator<std::string> i(std::cin), e ; i != e ; ++i) oss << *i << " ";
		in = oss.str();
	}

	dav::tokenizer::parse(in.cbegin(), in.cend(), tokens);
	dav::infixtopostfix::parse(tokens.cbegin(), tokens.cend(), postbuf);
	double v = dav::evaluator::parse(postbuf.cbegin(), postbuf.cend());

	td::cout << "= " << v << std::endl;

	return 0;
}
catch(const std::string& e)
{
	std::cerr << std::endl << "ERR: " << e << std::endl;
	return 1;
}

