#include <iostream>
#include <sstream>
#include <vector>
#include "lex.hpp"
#include "lib.hpp"
#include "Tokenizer.h"

// # HEADER # ==================================================================

#ifndef DAV_PARSER_TTABLE_H
#define DAV_PARSER_TTABLE_H

#include "analysis_io.hpp"

namespace dav
{
	namespace ttable
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

namespace dav { namespace ttable {

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

typedef String<'$'> Var0;
typedef String<'[', 'A', '-', 'Z', ']', '[', 'A', '-', 'Z', 'a', '-', 'z', '0', '-', '9', '_', ']', '*'> Var1;
typedef String<'='> Var2;
typedef String<','> Var3;
typedef String<'#'> Var4;
typedef String<'^'> Var5;
typedef String<'&'> Var6;
typedef String<'v'> Var7;
typedef String<'|'> Var8;
typedef String<'!'> Var9;
typedef String<'('> Var10;
typedef String<')'> Var11;

typedef Analysis
<
	MakeTypeList
	<
		MakeTypeList
		<
			MakeTypeList<Production<1>, Print<Var0>, End>
		>,
		MakeTypeList
		<
			MakeTypeList<Match<Var1>, PrintID, Literal<Var2>, Production<2>, Production<4>>
		>,
		MakeTypeList
		<
			MakeTypeList<Match<Var1>, PrintID, Production<3>>
		>,
		MakeTypeList
		<
			MakeTypeList<Literal<Var3>, Production<2>>,
			MakeTypeList<Literal<Var2>, Print<Var4>>
		>,
		MakeTypeList
		<
			MakeTypeList<Production<6>, Production<5>>
		>,
		MakeTypeList
		<
			MakeTypeList<Literal<Var5>, Production<6>, Print<Var6>, Production<5>>,
			MakeTypeList<Literal<Var7>, Production<6>, Print<Var8>, Production<5>>,
			MakeTypeList<Empty>
		>,
		MakeTypeList
		<
			MakeTypeList<Literal<Var9>, Production<7>, Print<Var9>>,
			MakeTypeList<Production<7>>
		>,
		MakeTypeList
		<
			MakeTypeList<Literal<Var10>, Production<4>, Literal<Var11>>,
			MakeTypeList<Match<Var1>, PrintID>
		>
	>
>
TTableParser;

void parse(Reader& in, Writer& out)
{
	TTableParser::parse(in);

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
//	dav::ttable::parse(buf.cbegin(), buf.cend(), post);

//	for(const std::string& s : post) std::cout << s << " ";

//	std::cout << std::endl;

//	return 0;
//}
//catch(const std::string& e)
//{
//	std::cerr << std::endl << "ERR: " << e << std::endl;
//}

