#include <iostream>
#include <vector>
#include <iterator>
#include "Tokenizer.h"
#include "TTable.h"
#include "Evaluator.h"

int main(int argc, char *argv[])
try
{
	std::vector<std::string> buf, post;
	std::string in;

	{
		std::ostringstream oss;
		for(std::istream_iterator<std::string> i(std::cin), e ; i != e ; ++i) oss << *i << " ";
		in = oss.str();
	}

	dav::tokenizer::parse(in.cbegin(), in.cend(), buf);
	dav::ttable::parse(buf.cbegin(), buf.cend(), post);

	dav::dil::evaluate(post.cbegin(), post.cend());

	return 0;
}
catch(const std::string& e)
{
	std::cerr << std::endl << "ERR: " << e << std::endl;
}

