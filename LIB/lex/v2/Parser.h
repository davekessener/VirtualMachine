#ifndef DAV_LEX_PARSER_H
#define DAV_LEX_PARSER_H

#include "analysis_io.hpp"

namespace dav
{
	namespace parser
	{
		using io::Reader;
		using io::Writer;

		void parse(Reader&, std::ostream&);

		template<typename I>
		inline void parse(I i1, I i2, std::ostream& out)
		{
			io::StringIterator<I> in(i1, i2);

			parse(in, out);
		}

		template<typename I>
		inline typename std::enable_if<IsIterable<DoDecay<I>>::value>::type
			parse(I&& in, std::ostream& out)
		{
			parse(in.cbegin(), in.cend(), out);
		}
	}
}

#endif

