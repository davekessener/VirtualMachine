#include "Reader.h"

namespace vm { namespace assembler {

class Reader::Impl
{
	public:
		Impl(std::istream&);
		~Impl( ) throw();
		const std::string getline( );
		void ungetline(const std::string&);
		bool empty( );
		int getCurrentLineNumber( ) const;
	private:
		const std::string doReadLine( );
		void fillBuffer( );
	private:
		std::istream &in_;
		int lc_;
		std::deque<std::string> buf_;
};

namespace
{
	void trim(std::string&);
}

// # ===========================================================================

Reader::Reader(std::istream& in) : impl_(new Impl(in))
{
}

Reader::~Reader(void) throw()
{
	delete impl_;
}

const std::string Reader::getline(void)
{
	return impl_->getline();
}

void Reader::ungetline(const std::string& line)
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

Reader::Impl::Impl(std::istream& in) : in_(in), lc_(0)
{
}

Reader::Impl::~Impl(void) throw()
{
}

const std::string Reader::Impl::getline(void)
{
	fillBuffer();

	std::string r;
	
	if(!buf_.empty())
	{
		r = buf_.front();
		buf_.pop_front();
	}

	return r;
}

void Reader::Impl::ungetline(const std::string& line)
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
		std::string l(doReadLine());
		trim(l);

		if(!l.empty()) buf_.push_back(l);
	}
}

// # ===========================================================================

namespace
{
	void trim(std::string& s)
	{
		std::string r, t;
		bool beg = false;

		for(auto i = s.cbegin() ; i != s.cend() ; ++i)
		{
			switch(*i)
			{
				case ' ':
				case '\t':
				case '\n':
				case '\0':
					if(beg)
					{
						t.push_back(' ');
					}
					break;
				default:
					r += t;
					r.push_back(*i);
					t.clear();
					beg = true;
					break;
			}
		}
	}
}

}}

