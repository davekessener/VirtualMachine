#include <cassert>
#include "Token.h"

namespace vm { namespace assembler {

class Token::Impl
{
	public:
		Impl(const std::string&, const std::string&, int, int);
		~Impl( ) throw();
		inline bool operator==(const std::string& s) const { return cont_ == s; }
	private:
		friend class Token;
		std::string cont_, fn_;
		int lc_, wc_;
};

// # ===========================================================================

Token::Impl::Impl(const std::string& c, const std::string& fn, int lc, int wc)
	: cont_(c), fn_(fn), lc_(lc), wc_(wc)
{
}

Token::Impl::~Impl(void) throw()
{
}

// # ---------------------------------------------------------------------------

// # ===========================================================================

Token::Token(void) : impl_(NULL)
{
}

Token::Token(Token&& t) : impl_(NULL)
{
	swap(t);
}

Token::Token(const Token& t) : impl_(t.impl_ ? new Impl(*t.impl_) : NULL)
{
}

Token::Token(const std::string& c, const std::string& fn, int lc, int wc)
	: impl_(new Impl(c, fn, lc, wc))
{
}

Token::~Token(void) throw()
{
	delete impl_;
}

Token& Token::operator=(Token&& t)
{
	swap(t);
	return *this;
}

Token& Token::operator=(const Token& t)
{
	Token s(t);
	swap(s);
	return *this;
}

void Token::swap(Token& t) throw()
{
	Impl *i = impl_;
	impl_ = t.impl_;
	t.impl_ = i;
}

bool Token::operator==(const Token& t) const
{
	return (impl_ && t.impl_) ? impl_->operator==(t.impl_->cont_) : false;
}

bool Token::operator==(const std::string& s) const
{
	return impl_ ? impl_->operator==(s) : false;
}

std::string& Token::str(void)
{
	assert(impl_);
	return impl_->cont_;
}

const char *Token::c_str(void) const
{
	assert(impl_);
	return impl_->cont_.c_str();
}

const std::string& Token::str(void) const
{
	assert(impl_);
	return impl_->cont_;
}

const std::string Token::filename(void) const
{
	assert(impl_);
	return impl_->fn_;
}

int Token::line(void) const
{
	assert(impl_);
	return impl_->lc_;
}

int Token::word(void) const
{
	assert(impl_);
	return impl_->wc_;
}

char Token::operator[](size_t idx) const
{
	return impl_->cont_.at(idx);
}

}}

