#ifndef DAV_PARSER_INFIXTOPOSTFIX_H
#define DAV_PARSER_INFIXTOPOSTFIX_H

#include <iostream>
#include <sstream>
#include <vector>
#include "lex.hpp"
#include "lib.hpp"
#include "Tokenizer.h"
#include "analysis_io.hpp"

namespace dav { namespace infixtopostfix {

using lex::Analysis;
using lex::Production;
using lex::Match;
using lex::Literal;
using lex::Print;
using lex::PrintID;
using lex::End;
using lex::Empty;
using io::Reader;
using io::Writer;

void parse(Reader&, Writer&);

template<typename O, typename C = io::WriteSTLContainer<DoDecay<O>>>
typename std::enable_if<!IsDerived<io::Writer, DoDecay<O>>::value>::type
	parse(io::Reader& in, O&& o)
{
	C out(o);
	parse(in, out);
}

template<typename I, typename O>
inline void parse(I i1, I i2, O&& out)
{
	io::StringIterator<I> in(i1, i2);
	parse(in, out);
}

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
typedef String<'E'> Var19;
typedef String<'N', 'E', 'G'> Var20;
typedef String<'('> Var21;
typedef String<')'> Var22;
typedef String<'p', 'i', '|', 'P', 'I'> Var23;
typedef String<'P', 'I'> Var24;
typedef String<'[', 'e', 'E', ']'> Var25;
typedef String<'[', '0', '-', '9', ']', '+', '(', '\\', '.', '[', '0', '-', '9', ']', '+', ')', '?', '(', '[', 'e', 'E', ']', '[', '+', '-', ']', '?', '[', '0', '-', '9', ']', '+', ')', '?'> Var26;

typedef Analysis
<
	MakeTypeList
	<
		MakeTypeList
		<
			MakeTypeList<Production<1>, End>
		>,
		MakeTypeList
		<
			MakeTypeList<Production<3>, Production<2>>
		>,
		MakeTypeList
		<
			MakeTypeList<Production<3>, Production<2>>,
			MakeTypeList<Empty>
		>,
		MakeTypeList
		<
			MakeTypeList<Production<5>, Production<4>>
		>,
		MakeTypeList
		<
			MakeTypeList<Literal<Var0>, Production<5>, Print<Var0>, Production<4>>,
			MakeTypeList<Literal<Var1>, Production<5>, Print<Var1>, Production<4>>,
			MakeTypeList<Empty>
		>,
		MakeTypeList
		<
			MakeTypeList<Production<7>, Production<6>>
		>,
		MakeTypeList
		<
			MakeTypeList<Literal<Var2>, Production<7>, Print<Var2>, Production<6>>,
			MakeTypeList<Literal<Var3>, Production<7>, Print<Var3>, Production<6>>,
			MakeTypeList<Empty>
		>,
		MakeTypeList
		<
			MakeTypeList<Production<9>, Production<8>>
		>,
		MakeTypeList
		<
			MakeTypeList<Literal<Var4>, Production<7>, Print<Var4>>,
			MakeTypeList<Empty>
		>,
		MakeTypeList
		<
			MakeTypeList<Literal<Var5>, Production<10>, Print<Var6>>,
			MakeTypeList<Literal<Var7>, Production<10>, Print<Var8>>,
			MakeTypeList<Literal<Var9>, Production<10>, Print<Var10>>,
			MakeTypeList<Literal<Var11>, Production<10>, Print<Var12>>,
			MakeTypeList<Literal<Var13>, Production<10>, Print<Var12>>,
			MakeTypeList<Literal<Var14>, Production<10>, Print<Var15>>,
			MakeTypeList<Literal<Var16>, Production<10>, Print<Var17>>,
			MakeTypeList<Literal<Var18>, Print<Var19>, Production<7>, Print<Var4>>,
			MakeTypeList<Production<10>>
		>,
		MakeTypeList
		<
			MakeTypeList<Literal<Var1>, Production<11>, Print<Var20>>,
			MakeTypeList<Production<11>>
		>,
		MakeTypeList
		<
			MakeTypeList<Literal<Var21>, Production<1>, Literal<Var22>>,
			MakeTypeList<Match<Var23>, Print<Var24>>,
			MakeTypeList<Match<Var25>, Print<Var19>>,
			MakeTypeList<Match<Var26>, PrintID>
		>
	>
>
InfixToPostfixParser;

void parse(Reader& in, Writer& out)
{
	InfixToPostfixParser::parse(in);

	lex::StringQueue::flush([&out](const std::string& s) { out.push(s); });
}

} }

//int main(int argc, char *argv[])
//try
//{
//	std::vector<std::string> buf, post;
//	std::string in;
//
//	{
//		std::ostringstream oss;
//		for(std::istream_iterator<std::string> i(std::cin), e ; i != e ; ++i) oss << *i << " ";
//		in = oss.str();
//	}
//
//	dav::tokenizer::parse(in.cbegin(), in.cend(), buf);
//	dav::infixtopostfix::parse(buf.cbegin(), buf.cend(), post);
//
//	for(const std::string& s : post) std::cout << s << " ";
//
//	std::cout << std::endl;
//
//	return 0;
//}
//catch(const std::string& e)
//{
//	std::cerr << std::endl << "ERR: " << e << std::endl;
//}

#endif
