#ifndef DAV_PARSER_INFIXTOPOSTFIX_H
#define DAV_PARSER_INFIXTOPOSTFIX_H

#include "analysis_io.hpp"

namespace dav
{
	namespace infixtopostfix
	{
		void parse(io::Reader&, io::Writer&);
		
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
	}
}

#endif

