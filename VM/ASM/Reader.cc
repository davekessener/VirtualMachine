#include <fstream>
#include <deque>
#include "Reader.h"
#include "stringtools.h"
#include "Logger.h"
#include "ASMException.h"

namespace vm { namespace assembler {

namespace
{
	Line make_line(const std::string&, const std::string&, int);
}

class Reader::Impl
{
	public:
		Impl(const std::string&);
		~Impl( ) throw();
		const Line getline( );
		void ungetline(const Line&);
		bool empty( );
		int getCurrentLineNumber( ) const;
	private:
		const std::string doReadLine( );
		void fillBuffer( );
	private:
		std::ifstream &in_;
		const std::string fn_;
		int lc_;
		std::deque<Line> buf_;
};

// # ===========================================================================

Reader::Reader(const std::string& fn) : impl_(new Impl(fn))
{
}

Reader::~Reader(void) throw()
{
	delete impl_;
}

const Line Reader::getline(void)
{
	return impl_->getline();
}

void Reader::ungetline(const Line& line)
{
	impl_->ungetline(line);
}

bool Reader::empty(void)
{
	return impl_->empty();
}

int Reader::getCurrentLineNumber(void) const
{
	return impl_->getCurrentLineNumber();
}

// # ===========================================================================

Reader::Impl::Impl(const std::string& fn) : in_(fn), fn_(fn), lc_(0)
{
	if(!in_.is_open()) MXT_LOGANDTHROW("ERR: Couldn't open file '%s'.", fn.c_str());
}

Reader::Impl::~Impl(void) throw()
{
}

const Line Reader::Impl::getline(void)
{
	fillBuffer();

	Line r;
	
	if(!buf_.empty())
	{
		r.swap(buf_.front());
		buf_.pop_front();
	}

	return r;
}

void Reader::Impl::ungetline(const Line& line)
{
	buf_.push_front(line);
}

bool Reader::Impl::empty(void)
{
	fillBuffer();

	return buf_.empty();
}

int Reader::Impl::getCurrentLineNumber(void) const
{
	return lc_;
}

// # ---------------------------------------------------------------------------

const std::string Reader::Impl::doReadLine(void)
{
	std::string l;
	
	std::getline(in_, l);
	
	lc_++;

	return l;
}

void Reader::Impl::fillBuffer(void)
{
	while(buf_.empty() && !in_.eof())
	{
		std::string line(doReadLine());
		Line l(make_line(line, fn_, lc_));

		if(static_cast<bool>(l)) buf_.push_back(l);
	}
}

// # ===========================================================================

namespace
{
	Line make_line(const std::string& line, const std::string& fn, int lc)
	{
		Line l(fn, lc);
	}
}

}}

