#include <iostream>
#include <sstream>
#include <vector>
#include "lex.hpp"
#include "lib.hpp"
#include "Tokenizer.h"

// # HEADER # ==================================================================

#ifndef DAV_PARSER_DEA_H
#define DAV_PARSER_DEA_H

#include "analysis_io.hpp"

namespace dav
{
	namespace dea
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

namespace dav { namespace dea {

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

typedef String<'{'> Var0;
typedef String<':'> Var1;
typedef String<'}'> Var2;
typedef String<']'> Var3;
typedef String<'<'> Var4;
typedef String<'['> Var5;
typedef String<','> Var6;
typedef String<';'> Var7;
typedef String<'*'> Var8;
typedef String<'>'> Var9;
typedef String<'[', 'a', '-', 'z', 'A', '-', 'Z', '0', '-', '9', ']', '[', 'a', '-', 'z', 'A', '-', 'Z', '0', '-', '9', ']', '*'> Var10;
typedef String<'D', 'E', 'A'> Var11;
typedef String<'^'> Var12;
typedef String<'F', 'R', 'O', 'M'> Var13;
typedef String<'T', 'Q'> Var14;
typedef String<'W', 'I', 'T', 'H'> Var15;
typedef String<'|'> Var16;
typedef String<'I', 'N', 'I', 'T', 'I', 'A', 'L'> Var17;
typedef String<'A', 'C', 'C', 'E', 'P', 'T', 'I', 'N', 'G'> Var18;
typedef String<'%'> Var19;

typedef Analysis
<
	MakeTypeList
	<
		MakeTypeList
		<
			MakeTypeList<Production<1>, Production<0>>,
			MakeTypeList<Production<12>, Production<0>>,
			MakeTypeList<End>
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
			MakeTypeList<Production<4>, Literal<Var0>, Print<Var1>, Production<6>, Literal<Var2>, Print<Var1>, Production<8>, Production<5>, Literal<Var3>, Print<Var4>>
		>,
		MakeTypeList
		<
			MakeTypeList<Literal<Var5>>
		>,
		MakeTypeList
		<
			MakeTypeList<Production<8>, Production<5>>,
			MakeTypeList<Empty>
		>,
		MakeTypeList
		<
			MakeTypeList<Production<11>, Production<7>>
		>,
		MakeTypeList
		<
			MakeTypeList<Literal<Var6>, Production<6>>,
			MakeTypeList<Empty>
		>,
		MakeTypeList
		<
			MakeTypeList<Production<10>, Production<11>, Production<9>, Literal<Var7>, Print<Var7>>
		>,
		MakeTypeList
		<
			MakeTypeList<Production<11>, Production<9>>,
			MakeTypeList<Empty>
		>,
		MakeTypeList
		<
			MakeTypeList<Literal<Var8>, Production<11>, Print<Var8>>,
			MakeTypeList<Literal<Var9>, Production<11>, Print<Var9>>,
			MakeTypeList<Production<11>>
		>,
		MakeTypeList
		<
			MakeTypeList<Match<Var10>, PrintID>
		>,
		MakeTypeList
		<
			MakeTypeList<Production<3>, Print<Var4>>
		>,
		MakeTypeList
		<
			MakeTypeList<Production<15>, Production<14>>
		>,
		MakeTypeList
		<
			MakeTypeList<Production<15>, Production<14>>,
			MakeTypeList<Empty>
		>,
		MakeTypeList
		<
			MakeTypeList<Literal<Var11>, Print<Var12>, Production<17>, Production<16>>
		>,
		MakeTypeList
		<
			MakeTypeList<Production<17>, Production<16>>,
			MakeTypeList<Empty>
		>,
		MakeTypeList
		<
			MakeTypeList<Literal<Var13>, Print<Var1>, Production<11>, Production<23>, Production<19>, Production<18>>
		>,
		MakeTypeList
		<
			MakeTypeList<Production<17>>,
			MakeTypeList<Empty>
		>,
		MakeTypeList
		<
			MakeTypeList<Literal<Var14>, Print<Var9>, Production<11>, Literal<Var15>, Print<Var16>, Production<21>, Production<20>>
		>,
		MakeTypeList
		<
			MakeTypeList<Production<19>>,
			MakeTypeList<Empty>
		>,
		MakeTypeList
		<
			MakeTypeList<Production<11>, Production<22>>
		>,
		MakeTypeList
		<
			MakeTypeList<Literal<Var6>, Print<Var16>, Production<21>, Production<22>>,
			MakeTypeList<Empty>
		>,
		MakeTypeList
		<
			MakeTypeList<Literal<Var17>, Print<Var8>>,
			MakeTypeList<Literal<Var18>, Print<Var19>>,
			MakeTypeList<Empty>
		>
	>
>
DEAParser;

void parse(Reader& in, Writer& out)
{
	DEAParser::parse(in);

	lex::StringQueue::flush([&out](const std::string& s) { out.push(s); });
}

} }

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
	dav::dea::parse(buf.cbegin(), buf.cend(), post);

	for(const std::string& s : post) std::cout << s << " ";

	std::cout << std::endl;

	return 0;
}
catch(const std::string& e)
{
	std::cerr << std::endl << "ERR: " << e << std::endl;
}

