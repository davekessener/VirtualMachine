#ifndef DAV_LEX_EVALUATOR_H
#define DAV_LEX_EVALUATOR_H

#include "analysis_io.hpp"

namespace dav
{
	namespace evaluator
	{
		typedef double number_t;

		number_t parse(io::Reader&);

		template<typename I>
		inline number_t parse(I i1, I i2)
		{
			io::StringIterator<I> in(i1, i2);

			return parse(in);
		}

		template<typename I>
		inline typename std::enable_if<IsIterable<DoDecay<I>>::value, number_t>::type
			parse(I&& in)
		{
			return parse(in.cbegin(), in.cend());
		}
	}
}

#endif

