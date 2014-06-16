#include "Reader.h"

namespace vm { namespace assembler {

class Reader::Impl
{
	public:
	private:
		friend class Reader;
};

// # ===========================================================================

// # ===========================================================================

Reader::Reader(void) : impl_(new Impl), ref_(new size_t)
{
	*ref_ = 1;
}

Reader::Reader(Reader&& r) : impl_(NULL), ref_(NULL)
{
	swap(r);
}

Reader::Reader(const Reader& r) : impl_(r.impl_), ref_(r.ref_)
{
	++*ref_;
}

Reader::~Reader(void) throw()
{
	if(ref_ && !--*ref_)
	{
		delete impl_;
	}
}

Reader& Reader::operator=(Reader&& r)
{
	swap(r);
	return *this;
}

Reader& Reader::operator=(const Reader& r)
{
	Reader t(r);
	swap(t);
	return *this;
}

void Reader::swap(Reader& r) throw()
{
	Impl *i = impl_;
	size_t *s = ref_;
	impl_ = r.impl_;
	ref_ = r.ref_;
	r.impl_ = i;
	r.ref_ = s;
}

}}

