#include <iostream>
#include <sstream>
#include <vector>
#include "lex.hpp"
#include "lib.hpp"
#include "Tokenizer.h"

// # HEADER # ==================================================================

#ifndef DAV_PARSER_TABLEFILL_H
#define DAV_PARSER_TABLEFILL_H

#include "analysis_io.hpp"

namespace dav
{
	namespace tablefill
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

namespace dav { namespace tablefill {

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

typedef String<'<'> Var0;
typedef String<'D', 'E', 'A'> Var1;
typedef String<'\n', '^'> Var2;
typedef String<'F', 'R', 'O', 'M'> Var3;
typedef String<':'> Var4;
typedef String<'T', 'O'> Var5;
typedef String<'>'> Var6;
typedef String<'W', 'I', 'T', 'H'> Var7;
typedef String<'|'> Var8;
typedef String<','> Var9;
typedef String<'I', 'N', 'I', 'T', 'I', 'A', 'L'> Var10;
typedef String<'*'> Var11;
typedef String<'A', 'C', 'C', 'E', 'P', 'T', 'I', 'N', 'G'> Var12;
typedef String<'%'> Var13;
typedef String<'[', 'a', '-', 'z', 'A', '-', 'Z', ']', '[', 'a', '-', 'z', 'A', '-', 'Z', '0', '-', '9', ']', '*'> Var14;
typedef String<'[', 'a', '-', 'z', 'A', '-', 'Z', '0', '-', '9', ']'> Var15;

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
			MakeTypeList<Production<3>, Production<2>>
		>,
		MakeTypeList
		<
			MakeTypeList<Production<3>, Production<2>>,
			MakeTypeList<Empty>
		>,
		MakeTypeList
		<
			MakeTypeList<Literal<Var1>, Print<Var2>, Production<5>, Production<4>>
		>,
		MakeTypeList
		<
			MakeTypeList<Production<5>, Production<4>>,
			MakeTypeList<Empty>
		>,
		MakeTypeList
		<
			MakeTypeList<Literal<Var3>, Print<Var4>, Production<12>, Production<11>, Production<7>, Production<6>>
		>,
		MakeTypeList
		<
			MakeTypeList<Production<5>>,
			MakeTypeList<Empty>
		>,
		MakeTypeList
		<
			MakeTypeList<Literal<Var5>, Print<Var6>, Production<12>, Literal<Var7>, Print<Var8>, Production<9>, Production<8>>
		>,
		MakeTypeList
		<
			MakeTypeList<Production<7>>,
			MakeTypeList<Empty>
		>,
		MakeTypeList
		<
			MakeTypeList<Production<13>, Production<10>>
		>,
		MakeTypeList
		<
			MakeTypeList<Literal<Var9>, Print<Var8>, Production<9>, Production<10>>,
			MakeTypeList<Empty>
		>,
		MakeTypeList
		<
			MakeTypeList<Literal<Var10>, Print<Var11>>,
			MakeTypeList<Literal<Var12>, Print<Var13>>,
			MakeTypeList<Empty>
		>,
		MakeTypeList
		<
			MakeTypeList<Match<Var14>, PrintID>
		>,
		MakeTypeList
		<
			MakeTypeList<Match<Var15>, PrintID>
		>
	>
>
TableFillParser;

void parse(Reader& in, Writer& out)
{
	TableFillParser::parse(in);

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
	dav::tablefill::parse(buf.cbegin(), buf.cend(), post);

	for(const std::string& s : post) std::cout << s << " ";

	std::cout << std::endl;

	return 0;
}
catch(const std::string& e)
{
	std::cerr << std::endl << "ERR: " << e << std::endl;
}

