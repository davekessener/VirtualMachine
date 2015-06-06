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
typedef String<'}'> Var1;
typedef String<':'> Var2;
typedef String<','> Var3;
typedef String<';'> Var4;
typedef String<'*'> Var5;
typedef String<'>'> Var6;
typedef String<'[', 'a', '-', 'z', 'A', '-', 'Z', '0', '-', '9', ']', '[', 'a', '-', 'z', 'A', '-', 'Z', '0', '-', '9', ']', '*'> Var7;

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
			MakeTypeList<Literal<Var0>, Production<3>, Literal<Var1>, Print<Var2>, Production<5>, Production<2>>
		>,
		MakeTypeList
		<
			MakeTypeList<Production<5>, Production<2>>,
			MakeTypeList<Empty>
		>,
		MakeTypeList
		<
			MakeTypeList<Production<8>, Production<4>>
		>,
		MakeTypeList
		<
			MakeTypeList<Literal<Var3>, Production<3>>,
			MakeTypeList<Empty>
		>,
		MakeTypeList
		<
			MakeTypeList<Production<7>, Production<8>, Production<6>, Literal<Var4>, Print<Var4>>
		>,
		MakeTypeList
		<
			MakeTypeList<Production<8>, Production<6>>,
			MakeTypeList<Empty>
		>,
		MakeTypeList
		<
			MakeTypeList<Literal<Var5>, Production<8>, Print<Var5>>,
			MakeTypeList<Literal<Var6>, Production<8>, Print<Var6>>,
			MakeTypeList<Production<8>>
		>,
		MakeTypeList
		<
			MakeTypeList<Match<Var7>, PrintID>
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

