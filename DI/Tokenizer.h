#ifndef DAV_TOKENIZER_H
#define DAV_TOKENIZER_H

#include <type_traits>
#include <lex/v2/analysis_io.hpp>

namespace dav
{
	namespace tokenizer
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

