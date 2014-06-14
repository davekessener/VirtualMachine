#include "Preprocessor.h"
#include <deque>
#include <cassert>

namespace vm { namespace assembler {

class Preprocessor::Impl
{
	public:
		Impl( );
		~Impl( ) throw();
		void feed(const std::string&);
		inline bool ready( ) const { return !buf_.empty(); }
		inline const std::string get( ) 
			{ assert(ready()); std::string r(buf_.front()); buf_.pop_front(); return r; }
	private:
		std::deque<std::string> buf_;
};

// # ===========================================================================

Preprocessor::Preprocessor(void) : impl_(new Impl)
{
}

Preprocessor::~Preprocessor(void) throw()
{
	delete impl_;
}

void Preprocessor::feed(const std::string& line)
{
	impl_->feed(line);
}

const std::string Preprocessor::get(void)
{
	return impl_->get();
}

bool Preprocessor::ready(void) const
{
	return impl_->ready();
}

void Preprocessor::clear(void)
{
	impl_->~Impl();
	new(impl_) Impl;
}

// # ===========================================================================

Preprocessor::Impl::Impl(void)
{
}

Preprocessor::Impl::~Impl(void) throw()
{
}

void Preprocessor::Impl::feed(const std::string& line)
{
	buf_.push_back(line);
}

}}

