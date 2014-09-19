#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include "lex.h"
#include <dav/Logger.h>
#include <boost/lexical_cast.hpp>
#include "Executor.hpp"
#include "Tokenizer.hpp"
#include "Parser.hpp"

using namespace dav;

using boost::lexical_cast;

number_t evaluate(const std::string& ex)
{
	std::vector<std::string> term(ex.size());
	std::copy(ex.cbegin(), ex.cend(), term.begin());

	Printer p;
	Tokenizer::parse(term.cbegin(), term.cend(), p);

	SVec sv;
	Parser::parse(p.cbegin(), p.cend(), sv);

	StackHolder sh;
	Evaluator::parse(sv.cbegin(), sv.cend(), sh);

	auto &s(sh.get());

	if(s.size() != 1) throw std::string("ERR: Invalid stack size! (" + lexical_cast<std::string>(s.size()) + ")");

	return s.top();
}

int main(int argc, char *argv[])
try
{
	std::vector<std::string> args(argv, argv + argc);

	std::string old_s("");
	number_t v(0);

	while(!std::cin.eof())
	{
		std::string s("");

		std::getline(std::cin, s);

		if(s.empty()) s = old_s;

		if(s.empty() || s[0] == '#') continue;

		old_s = s;

		s = MXT_REGEX_SCOPE::regex_replace(s, MXT_REGEX_SCOPE::regex("\\$"), lexical_cast<std::string>(v));

		v = evaluate(s);

		std::cout << "= " << v << std::endl;
	}

	return 0;
}
catch(const std::string& e)
{
	std::cerr<< e << std::endl;
}

