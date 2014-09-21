#ifndef DAV_PARSER_INFIXTOPOSTFIX_H
#define DAV_PARSER_INFIXTOPOSTFIX_H

#include <iostream>
#include <vector>
#include <iterator>
#include "lex.h"
#include "Tokenizer.hpp"

namespace dav { namespace infixtopostfix {
typedef String<'+'> Var0;
typedef String<'-'> Var1;
typedef String<'*'> Var2;
typedef String<'/'> Var3;
typedef String<'^'> Var4;
typedef String<'s', 'i', 'n'> Var5;
typedef String<'S', 'I', 'N'> Var6;
typedef String<'c', 'o', 's'> Var7;
typedef String<'C', 'O', 'S'> Var8;
typedef String<'t', 'a', 'n'> Var9;
typedef String<'T', 'A', 'N'> Var10;
typedef String<'l', 'o', 'g'> Var11;
typedef String<'L', 'G'> Var12;
typedef String<'l', 'g'> Var13;
typedef String<'l', 'n'> Var14;
typedef String<'L', 'N'> Var15;
typedef String<'s', 'q', 'r', 't'> Var16;
typedef String<'S', 'Q', 'R', 'T'> Var17;
typedef String<'e', 'x', 'p'> Var18;
typedef String<'2', '.', '7', '6'> Var19;
typedef String<'N', 'E', 'G'> Var20;
typedef String<'('> Var21;
typedef String<')'> Var22;
typedef String<'P', 'I'> Var23;
typedef String<'3', '.', '1', '4', '1'> Var24;
typedef String<'E'> Var25;
typedef String<'[', '0', '-', '9', ']', '+', '(', '\\', '.', '[', '0', '-', '9', ']', '+', ')', '?', '(', '[', 'e', 'E', ']', '[', '+', '-', ']', '?', '[', '0', '-', '9', ']', '+', ')', '?'> Var26;

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
			MakeTypeList<Translation<3>, Translation<2>>
		>,
		MakeTypeList
		<
			MakeTypeList<Translation<3>, Translation<2>>,
			MakeTypeList<Empty>
		>,
		MakeTypeList
		<
			MakeTypeList<Translation<5>, Translation<4>>
		>,
		MakeTypeList
		<
			MakeTypeList<Literal<Var0>, Translation<5>, Print<Var0>, Translation<4>>,
			MakeTypeList<Literal<Var1>, Translation<5>, Print<Var1>, Translation<4>>,
			MakeTypeList<Empty>
		>,
		MakeTypeList
		<
			MakeTypeList<Translation<7>, Translation<6>>
		>,
		MakeTypeList
		<
			MakeTypeList<Literal<Var2>, Translation<7>, Print<Var2>, Translation<6>>,
			MakeTypeList<Literal<Var3>, Translation<7>, Print<Var3>, Translation<6>>,
			MakeTypeList<Empty>
		>,
		MakeTypeList
		<
			MakeTypeList<Translation<9>, Translation<8>>
		>,
		MakeTypeList
		<
			MakeTypeList<Literal<Var4>, Translation<7>, Print<Var4>>,
			MakeTypeList<Empty>
		>,
		MakeTypeList
		<
			MakeTypeList<Literal<Var5>, Translation<10>, Print<Var6>>,
			MakeTypeList<Literal<Var7>, Translation<10>, Print<Var8>>,
			MakeTypeList<Literal<Var9>, Translation<10>, Print<Var10>>,
			MakeTypeList<Literal<Var11>, Translation<10>, Print<Var12>>,
			MakeTypeList<Literal<Var13>, Translation<10>, Print<Var12>>,
			MakeTypeList<Literal<Var14>, Translation<10>, Print<Var15>>,
			MakeTypeList<Literal<Var16>, Translation<10>, Print<Var17>>,
			MakeTypeList<Literal<Var18>, Print<Var19>, Translation<7>, Print<Var4>>,
			MakeTypeList<Translation<10>>
		>,
		MakeTypeList
		<
			MakeTypeList<Literal<Var1>, Translation<11>, Print<Var20>>,
			MakeTypeList<Translation<11>>
		>,
		MakeTypeList
		<
			MakeTypeList<Literal<Var21>, Translation<1>, Literal<Var22>>,
			MakeTypeList<Literal<Var23>, Print<Var24>>,
			MakeTypeList<Literal<Var25>, Print<Var19>>,
			MakeTypeList<ID<Var26>, PrintID>
		>
	>
>
InfixToPostfixParser;

} }

int main(int argc, char *argv[])
{
	using dav::Printer;
	using dav::Tokenizer;
	using dav::infixtopostfix::InfixToPostfixParser;

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
	InfixToPostfixParser::parse(printer.cbegin(), printer.cend(), std::cout);

	std::cout << std::endl;

	return 0;
}

#endif
