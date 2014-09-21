#ifndef DAV_PARSER_EVALPOSTFIX_H
#define DAV_PARSER_EVALPOSTFIX_H

#include <iostream>
#include <vector>
#include <iterator>
#include "lex.h"
#include "Tokenizer.hpp"

namespace dav { namespace evalpostfix {
typedef String<'[', '0', '-', '9', ']', '+', '(', '\\', '.', '[', '0', '-', '9', ']', '+', ')', '?', '(', '[', 'e', 'E', ']', '[', '+', '-', ']', '?', '[', '0', '-', '9', ']', '+', ')', '?'> Var0;
typedef String<'(', '\\', '+', '|', '\\', '-', '|', '\\', '*', '|', '\\', '/', '|', '\\', '^', '|', 'S', 'I', 'N', '|', 'C', 'O', 'S', '|', 'T', 'A', 'N', '|', 'L', 'G', '|', 'L', 'N', '|', 'S', 'Q', 'R', 'T', '|', 'N', 'E', 'G', ')'> Var1;

template<typename SymTable, typename Output>
struct CustomHook0
{
	static void run(SymTable& symtable, Output& output)
	{
		MyStack::push(lexical_cast<double>(symtable.at("*")));
	}
};

template<typename SymTable, typename Output>
struct CustomHook1
{
	static void run(SymTable& symtable, Output& output)
	{
		MyStack::eval(symtable.at("*"));
	}
};

typedef lex::Parser
<
	MakeTypeList
	<
		MakeTypeList
		<
			MakeTypeList<Translation<1>, End>
		>,
		MakeTypeList
		<
			MakeTypeList<Translation<2>, Translation<1>>,
			MakeTypeList<Empty>
		>,
		MakeTypeList
		<
			MakeTypeList<Literal<Var0>, Hook<CustomHook0>>,
			MakeTypeList<Literal<Var1>, Hook<CustomHook1>>
		>
	>
>
EvalPostfixParser;

} }

int main(int argc, char *argv[])
{
	using dav::Printer;
	using dav::Tokenizer;
	using dav::evalpostfix::EvalPostfixParser;

	typedef std::vector<std::string> svec;
	typedef std::istream_iterator<std::string> iiter;

	svec args(argv, argv + argc);
	svec input;

	{
		std::stringstream ss;
		std::for_each(iiter(std::cin), iiter(), [&ss](const std::string& s) { ss << s << " "; });
		std::string s(ss.str());
		input.resize(s.size());
		std::copy(s.cbegin(), s.cend(), input.begin());
	}

	Printer printer;

	Tokenizer::parse(input.cbegin(), input.cend(), printer);
	EvalPostfixParser::parse(printer.cbegin(), printer.cend(), std::cout);

	std::cout << std::endl;

	return 0;
}

#endif
