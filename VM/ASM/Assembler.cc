#include <iostream>
#include <vector>
#include <map>
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
#include "Tokenizer.h"
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
		SymTable sym_;
		Preprocessor pre_;
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
	LOG("Assembling file '%s' [...]", line.c_str());

	pre_.feed(Tokenizer(line));

	while(pre_.ready())
	{
		process(pre_.get());
	}

	finalize();

	LOG("[DONE]");
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

	sym_["$$"] = pos_;
	
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

	LOG("Processing line '%s'", line.str().c_str());

	switch(line[0][0])
	{
		case '.':
			LOG("Directive detected! ['%s']", line.str().c_str());
			t.reset(Directives::process(line));
			break;
		case ':':
			if(line.size() > 1) MXT_LOGANDTHROW_T(line[1], "ERR: Illegal: Trailing tokens.");
			LOG("Label '%s' detected.", line[0].str().c_str());
			t.reset(new Marker(line[0].str()));
			sym_[line[0].str()] = pos_;
			break;
		default:
			t.reset(ins_.translate(line));
			break;
	}

	if(static_cast<bool>(t))
	{
		ops_.push_back(t);
		pos_ += t->size();
		LOG("Line '%s' translated.", line.str().c_str());
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
		os.write(reinterpret_cast<const char *>(&v), sizeof(v));
		LOG("!OUT! '%04x'", v);
	}
}

}}

