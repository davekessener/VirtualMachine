#include <iostream>
#include <sstream>
#include <vector>
#include "lex.hpp"
#include "lib.hpp"
#include "Tokenizer.h"

// # HEADER # ==================================================================

#ifndef DAV_PARSER_INFIXTOPOSTFIX_H
#define DAV_PARSER_INFIXTOPOSTFIX_H

#include "analysis_io.hpp"

namespace dav
{
	namespace infixtopostfix
	{
		void parse(io::Reader&, io::Writer&);

		template<typename O, typename C = io::WriteSTLContainer<DoDecay<O>>>
		inline typename std::enable_if<!IsDerived<io::Writer, DoDecay<O>>::value>::type
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

		template<typename I, typename O>
		inline typename std::enable_if<IsIterable<DoDecay<I>>::value>::type
			parse(I&& i, O&& o)
		{
			parse(i.cbegin(), i.cend(), o);
		}
	}
}

#endif

// # ===========================================================================

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

typedef String<'+'> Var0;
typedef String<'-'> Var1;
typedef String<'*'> Var2;
typedef String<'/'> Var3;
typedef String<'^'> Var4;
typedef String<'N', 'E', 'G'> Var5;
typedef String<'s', 'i', 'n'> Var6;
typedef String<'S', 'I', 'N'> Var7;
typedef String<'c', 'o', 's'> Var8;
typedef String<'C', 'O', 'S'> Var9;
typedef String<'t', 'a', 'n'> Var10;
typedef String<'T', 'A', 'N'> Var11;
typedef String<'l', 'o', 'g'> Var12;
typedef String<'L', 'G'> Var13;
typedef String<'l', 'g'> Var14;
typedef String<'l', 'n'> Var15;
typedef String<'L', 'N'> Var16;
typedef String<'s', 'q', 'r', 't'> Var17;
typedef String<'S', 'Q', 'R', 'T'> Var18;
typedef String<'e', 'x', 'p'> Var19;
typedef String<'E'> Var20;
typedef String<'%'> Var21;
typedef String<'1', '0', '0'> Var22;
typedef String<'('> Var23;
typedef String<')'> Var24;
typedef String<'p', 'i', '|', 'P', 'I'> Var25;
typedef String<'P', 'I'> Var26;
typedef String<'[', 'e', 'E', ']'> Var27;
typedef String<'[', '0', '-', '9', ']', '+', '(', '\\', '.', '[', '0', '-', '9', ']', '+', ')', '?', '(', '[', 'e', 'E', ']', '[', '+', '-', ']', '?', '[', '0', '-', '9', ']', '+', ')', '?'> Var28;

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
			MakeTypeList<Literal<Var1>, Production<10>, Print<Var5>>,
			MakeTypeList<Production<10>>
		>,
		MakeTypeList
		<
			MakeTypeList<Literal<Var6>, Production<11>, Print<Var7>>,
			MakeTypeList<Literal<Var8>, Production<11>, Print<Var9>>,
			MakeTypeList<Literal<Var10>, Production<11>, Print<Var11>>,
			MakeTypeList<Literal<Var12>, Production<11>, Print<Var13>>,
			MakeTypeList<Literal<Var14>, Production<11>, Print<Var13>>,
			MakeTypeList<Literal<Var15>, Production<11>, Print<Var16>>,
			MakeTypeList<Literal<Var17>, Production<11>, Print<Var18>>,
			MakeTypeList<Literal<Var19>, Print<Var20>, Production<7>, Print<Var4>>,
			MakeTypeList<Production<11>>
		>,
		MakeTypeList
		<
			MakeTypeList<Production<13>, Production<12>>
		>,
		MakeTypeList
		<
			MakeTypeList<Literal<Var21>, Print<Var22>, Print<Var3>>,
			MakeTypeList<Empty>
		>,
		MakeTypeList
		<
			MakeTypeList<Literal<Var23>, Production<1>, Literal<Var24>>,
			MakeTypeList<Match<Var25>, Print<Var26>>,
			MakeTypeList<Match<Var27>, Print<Var20>>,
			MakeTypeList<Match<Var28>, PrintID>
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

//	{
//		std::ostringstream oss;
//		for(std::istream_iterator<std::string> i(std::cin), e ; i != e ; ++i) oss << *i << " ";
//		in = oss.str();
//	}

//	dav::tokenizer::parse(in.cbegin(), in.cend(), buf);
//	dav::infixtopostfix::parse(buf.cbegin(), buf.cend(), post);

//	for(const std::string& s : post) std::cout << s << " ";

//	std::cout << std::endl;

//	return 0;
//}
//catch(const std::string& e)
//{
//	std::cerr << std::endl << "ERR: " << e << std::endl;
//}

