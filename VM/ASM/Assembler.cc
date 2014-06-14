#include "Assembler.h"
#include <vector>
#include <cassert>
#include "Reader.h"
#include "Preprocessor.h"
#include "Logger.h"

namespace vm { namespace assembler {

class Assembler::Impl
{
	typedef std::vector<BYTE> cont_t;

	public:
		Impl( );
		~Impl( ) throw();
		void feed(const std::string&);
		inline bool ready( ) const { return !buf_.empty(); }
		inline cont_t::const_iterator begin( ) const { return buf_.cbegin(); }
		inline cont_t::const_iterator end( ) const { return buf_.cend(); }
		void init( );
		void finalize( );
	private:
		void process(const std::string&);
	private:
		Preprocessor pre_;
		cont_t buf_;
};

// # ===========================================================================

Assembler::Impl::Impl(void)
{
}

Assembler::Impl::~Impl(void) throw()
{
}

void Assembler::Impl::feed(const std::string& line)
{
	pre_.feed(line);

	while(pre_.ready())
	{
		process(pre_.get());
	}
}

void Assembler::Impl::init(void)
{
	pre_.clear();
	buf_.clear();
}

void Assembler::Impl::finalize(void)
{
}

// # ---------------------------------------------------------------------------

void Assembler::Impl::process(const std::string& line)
{	
	LOG(">%s", line.c_str());
}

// # ===========================================================================

Assembler::Assembler(void) : impl_(new Impl)
{
}

Assembler::~Assembler(void) throw()
{
	delete impl_;
}

void Assembler::assemble(Reader& r)
{
	impl_->init();

	while(!r.empty())
	{
		impl_->feed(r.getline());
	}

	impl_->finalize();
}

void Assembler::out(std::ostream& os)
{
	assert(impl_->ready());

	for(const BYTE v : *impl_)
	{
		os << v;
	}
}

}}

