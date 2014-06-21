#include <deque>
#include <fstream>
#include <memory>
#include <cassert>
#include "Tokenizer.h"
#include "Line.h"
#include "Logger.h"
#include "ASMException.h"

namespace vm { namespace assembler {

class TImpl
{
	public:
		TImpl(const std::string&);
		~TImpl( ) throw();
		inline const std::string& filename( ) const { return fn_; }
		inline size_t curLine( ) const { return lc_; }
		inline bool empty( ) { fillBuffer(); return buffer_.empty(); }
		Line getline( );
	private:
		void fillBuffer( );
		const std::string doReadLine( );
	private:
		friend class Tokenizer;
		std::ifstream in_;
		const std::string fn_;
		size_t lc_;
		std::deque<Line> buffer_;
};

class Tokenizer::Impl
{
	public:
		Impl(const std::string& s) : impl_(new TImpl(s)) { }
	private:
		friend class Tokenizer;
		std::shared_ptr<TImpl> impl_;
};

// # ===========================================================================

TImpl::TImpl(const std::string& fn) : in_(fn), fn_(fn), lc_(0)
{
	if(!in_.is_open()) MXT_LOGANDTHROW("ERR: Couldn't open file '%s'!", fn.c_str());
}

TImpl::~TImpl(void) throw()
{
	in_.close();
}

Line TImpl::getline(void)
{
	fillBuffer();

	if(empty()) MXT_LOGANDTHROW("ERR: In file '%s':%lu "
		"tried to read even though buffer is empty!", fn_.c_str(), lc_);
	
	Line l(buffer_.front());

	buffer_.pop_front();

	return l;
}

// # ---------------------------------------------------------------------------

void TImpl::fillBuffer(void)
{
	while(buffer_.empty() && !in_.eof())
	{
		Line l(doReadLine(), fn_, lc_);

		if(!l.empty()) buffer_.push_back(l);
	}
}

const std::string TImpl::doReadLine(void)
{
	std::string s;

	std::getline(in_, s);

	++lc_;

	return s;
}

// # ===========================================================================

Tokenizer::Tokenizer(void) : impl_(NULL)
{
}

Tokenizer::Tokenizer(Tokenizer&& t) : impl_(NULL)
{
	swap(t);
}

Tokenizer::Tokenizer(const Tokenizer& t) : impl_(t.impl_ ? new Impl(*t.impl_) : NULL)
{
}

Tokenizer::Tokenizer(const std::string& s) : impl_(new Impl(s))
{
}

Tokenizer::~Tokenizer(void) throw()
{
	delete impl_;
}

Tokenizer& Tokenizer::operator=(Tokenizer&& t)
{
	swap(t);
	return *this;
}

Tokenizer& Tokenizer::operator=(const Tokenizer& t)
{
	Tokenizer tt(t);
	swap(tt);
	return *this;
}

Tokenizer& Tokenizer::operator>>(Line& line)
{
	assert(impl_);

	line = getline();

	return *this;
}

Line Tokenizer::getline(void)
{
	assert(impl_);

	return impl_->impl_->getline();
}

const std::string& Tokenizer::filename(void) const
{
	assert(impl_);

	return impl_->impl_->fn_;
}

void Tokenizer::swap(Tokenizer& t) throw()
{
	Impl *i = impl_;
	impl_ = t.impl_;
	t.impl_ = i;
}

bool Tokenizer::ready(void) const
{
	return impl_ && !impl_->impl_->empty();
}

}}

