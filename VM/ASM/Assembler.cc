#include <iostream>
#include <vector>
#include <memory>
#include <cassert>
#include "Assembler.h"
#include "Reader.h"
#include "Instructions.h"
#include "Preprocessor.h"
#include "Logger.h"
#include "Directives.h"
#include "Marker.h"
#include "SymTable.h"
#include "Logger.h"
#include "ASMException.h"
#include "stringtools.h"

namespace vm { namespace assembler {

class Assembler::Impl
{
	typedef std::shared_ptr<Opcode> op_t;
	typedef std::vector<WORD> cont_vec;
	typedef std::vector<op_t> op_vec;

	public:
		Impl(std::istream&);
		~Impl( ) throw();
		void assemble(const std::string&);
		inline bool ready( ) const { return !buf_.empty(); }
		inline cont_vec::const_iterator begin( ) const { return buf_.cbegin(); }
		inline cont_vec::const_iterator end( ) const { return buf_.cend(); }
		void clear( );
		void process(const Line&);
	private:
		void finalize( );
	private:
		int pos_;
		Instructions ins_;
		Preprocessor pre_;
		SymTable sym_;
		cont_vec buf_;
		op_vec ops_;
};

// # ===========================================================================

Assembler::Impl::Impl(std::istream& is) : ins_(is)
{
}

Assembler::Impl::~Impl(void) throw()
{
}

void Assembler::Impl::assemble(const std::string& line)
{
	pre_.feed(line);

	while(pre_.ready())
	{
		process(pre_.get());
	}
}

void Assembler::Impl::clear(void)
{
	pos_ = 0;
	pre_.clear();
	sym_.clear();
	buf_.clear();
	ops_.clear();
}

void Assembler::Impl::finalize(void)
{
	buf_.clear();
	
	for(op_t p : ops_)
	{
		buf_ << p->evaluate(sym_, buf_.size());
	}

	ops_.clear();
}

// # ---------------------------------------------------------------------------

void Assembler::Impl::process(const Line& line)
{
	op_t t;

	switch(line[0][0])
	{
		case '.':
			LOG("Directive detected! ['%s']", line.str());
			t.reset(Directives::process(line));
			break;
		case ':':
			if(line.count() > 1) MXT_LOGANDTHROW_T(line[1], "ERR: Illegal: Trailing tokens.");
			LOG("Label '%s' detected.", line[0].str());
			t.reset(new Marker(line[0]));
			sym_.insert(line[0].str(), pos_);
			break;
		default:
			t.reset(ins_.translate(line));
			break;
	}

	if(static_cast<bool>(t))
	{
		ops_.push_back(t);
		pos_ += t->size();
	}
	else
	{
		MXT_LOGANDTHROW("Couldn't translate line '%s' in '%s':%d", 
			line.str().c_str(), line.filename().c_str(), line.line());
	}
}

// # ===========================================================================

Assembler::Assembler(std::istream& is) : impl_(new Impl(is))
{
}

Assembler::~Assembler(void) throw()
{
	delete impl_;
}

void Assembler::assemble(const std::string& fn)
{
	try
	{
		impl_->assemble(fn);
	}
	catch(const ASMException& e)
	{
		std::cerr << "ERR: '" << e.what() << "'" << std::endl;
		impl_->clear();

		throw;
	}
}

void Assembler::out(std::ostream& os)
{
	assert(impl_->ready());

	for(const WORD v : *impl_)
	{
		os << v;
	}
}

}}

