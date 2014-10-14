#ifndef DAV_LEX_ANALYSISIO_H
#define DAV_LEX_ANALYSISIO_H

#include <boost/lexical_cast.hpp>
#include "io.hpp"

namespace dav
{
	namespace io
	{
		typedef ReadContainer<std::string> Reader;
		typedef WriteContainer<std::string> Writer;

		template<typename I>
		struct StringIterator : public ReadContainer<std::string>
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

	}
}

#endif

