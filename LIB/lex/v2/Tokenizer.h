#ifndef DAV_TOKENIZER_H
#define DAV_TOKENIZER_H

#include <boost/lexical_cast.hpp>
#include "io.hpp"

namespace dav
{
	namespace tokenizer
	{
		typedef io::ReadContainer<std::string> Reader;
		typedef io::WriteContainer<std::string> Writer;

		template<typename I>
		struct StringIterator : public io::ReadContainer<std::string>
		{
			public:
				StringIterator(const I& i1, const I& i2) : i1_(i1), i2_(i2), p_(0) { }
				virtual bool empty( ) const { return i1_ == i2_; }
				virtual size_t pos( ) const { return p_; }
				virtual std::string& top( )	{ assert(i1_!=i2_); return buf_ = boost::lexical_cast<std::string>(*i1_); }
				virtual void pop( ) { assert(i1_!=i2_); ++i1_; ++p_; }
			private:
				I i1_, i2_;
				size_t p_;
				std::string buf_;
		};

		template<typename I>
		inline void parse(I i1, I i2, Writer& out)
		{
			StringIterator<I> in(i1, i2);

			parse(in, out);
		}

		void parse(Reader&, Writer&);
	}
}

#endif

