#ifndef LIB_INSTREAM_H
#define LIB_INSTREAM_H

namespace lib
{
	namespace io
	{
		template<typename I, bool I::check()>
		class in_stream
		{
			public:
				in_stream(I& i) : s_(i) { }
				in_stream(in_stream& i) : s_(i.s_) { }
				explicit operator bool( ) const { return s_::check(); }

			private:
				I &s_;
		};
	}
}

#endif

