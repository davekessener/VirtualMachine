#include <sstream>
#include <exception>
#include "Line.h"
#include "Logger.h"
#include "ASMException.h"
#include "stringtools.h"

namespace vm { namespace assembler {

class Line::Impl
{
	public:
		Impl(const std::string&, int);
		~Impl( ) throw();
		void append(const std::string&);
		void append(const Token&);
	private:
		friend class Line;
		const std::string fn_;
		const int lc_;
		tok_vec toks_;
		int wc_;
};

// # ===========================================================================

Line::Impl::Impl(const std::string& fn, int lc) : fn_(fn), lc_(lc), wc_(0)
{
}

Line::Impl::~Impl(void) throw()
{
}

void Line::Impl::append(const std::string& word)
{
	toks_.push_back(Token(word, fn_, lc_, wc_));
	++wc_;
}

void Line::Impl::append(const Token& t)
{
	toks_.push_back(t);
	wc_ = t.word() + t.str().size();
}

// # ---------------------------------------------------------------------------

// # ===========================================================================

Line::Line(void) : impl_(NULL)
{
}

Line::Line(Line&& l) : impl_(NULL)
{
	swap(l);
}

Line::Line(const Line& l) : impl_(l.impl_ ? new Impl(*l.impl_) : NULL)
{
}

Line::Line(const std::string& fn, int lc) : impl_(new Impl(fn, lc))
{
}

Line::Line(const std::string& cont, const std::string& fn, int lc) : impl_(new Impl(fn, lc))
{
	std::string c(cont);
	int wc = 0;

	try
	{
		while(!c.empty())
		{
			std::string t(token_extract(c, wc));

			if(t.empty()) break;

			impl_->append(Token(t, fn, lc, wc));

			wc += t.size();
		}
	}
	catch(const char *e)
	{
		MXT_LOGANDTHROW("%s [['%s':%d]]", e, fn.c_str(), lc);
	}
}

Line::~Line(void) throw()
{
	delete impl_;
}

Line& Line::operator=(Line&& l)
{
	swap(l);
	return *this;
}

Line& Line::operator=(const Line& l)
{
	Line t(l);
	swap(t);
	return *this;
}

void Line::swap(Line& l) throw()
{
	Impl *i = impl_;
	impl_ = l.impl_;
	l.impl_ = i;
}

Line& Line::operator+=(const std::string& line)
{
	impl_->append(line);
	return *this;
}

Line& Line::insert(const std::vector<std::string>& lines)
{
	for(const std::string &l : lines)
	{
		impl_->append(l);
	}

	return *this;
}

Line::iterator Line::begin(void)
{
	return impl_->toks_.begin();
}

Line::iterator Line::end(void)
{
	return impl_->toks_.end();
}

Line::const_iterator Line::cbegin(void) const
{
	return impl_->toks_.cbegin();
}

Line::const_iterator Line::cend(void) const
{
	return impl_->toks_.cend();
}

const std::string Line::str(void) const
{
	std::ostringstream oss;

	for(auto i = impl_->toks_.begin() ; i != impl_->toks_.end() ; ++i)
	{
		if(i != impl_->toks_.begin()) oss << " ";
		oss << i->str();
	}

	return oss.str();
}

const std::string Line::filename(void) const
{
	return impl_->fn_;
}

int Line::line(void) const
{
	return impl_->lc_;
}

Token& Line::operator[](size_t idx)
{
	return impl_->toks_.at(idx);
}

const Token& Line::operator[](size_t idx) const
{
	return impl_->toks_.at(idx);
}

size_t Line::size(void) const
{
	return impl_->toks_.size();
}

}}

