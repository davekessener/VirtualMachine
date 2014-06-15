#include <deque>
#include <cassert>
#include "Preprocessor.h"
#include "Logger.h"
#include "ASMException.h"

namespace vm { namespace assembler {

class Preprocessor::Impl
{
	public:
		Impl( );
		~Impl( ) throw();
		void feed(const std::string&);
		inline bool ready( ) const { return !buf_.empty(); }
		inline Line get( ) 
			{ assert(ready()); Line l(buf_.front()); buf_.pop_front(); return l; }
	private:
		std::deque<std::string> buf_;
};

// # ===========================================================================

Preprocessor::Impl::Impl(void)
{
}

Preprocessor::Impl::~Impl(void) throw()
{
}

void Preprocessor::Impl::feed(const std::string& line)
{
	buf_.push_back(shorten(line));
}

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

namespace
{
	std::string shorten(const std::string& s)
	{
		std::string r;
		bool b = false, skip = false, malform = false;
		char ins = '\0';

		for(char c : s)
		{
			if(ins)
			{
				if(skip)
				{
					skip = false;
				}
				else
				{
					if(c == '\\')
					{
						skip = true;
					}
					else if(c == ins)
					{
						ins = false;
						b = false;
						malform = false;
					}

					if(malform) MXT_LOGANDTHROW("ERR: Badly formed expression: '%s'", s.c_str());
				}

				if(c != ins && ins == '\'') malform = true;
				
				r.push_back(c);
			}
			else if(c == ' ' || c == '\t' || c == '\n' || c == '\0')
			{
				if(!b) r.push_back(' ');
				b = true;
			}
			else if(c == '"' || c == '\'')
			{
				r.push_back(ins = c);
			}
			else
			{
				r.push_back(c);
				b = false;
			}
		}

		return r;
	}
}

}}

