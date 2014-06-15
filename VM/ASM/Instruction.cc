#include <iostream>
#include "Instruction.h"
#include "SymTable.h"
#include "Evaluator.h"
#include "OpTemplate.h"

namespace vm { namespace assembler {

class Instruction::Impl
{
	public:
		Impl(const OpTemplate&, const arg_vec&);
		~Impl( ) throw();
		void out(std::ostream&);
		int size( ) const;
		void evaluate(op_proxy&, SymTable&, int) const;
	private:
		OpTemplate op_;
		arg_vec args_;
};

// # ===========================================================================

Instruction::Impl::Impl(const OpTemplate& op, const arg_vec& vec) : op_(op), args_(vec)
{
}

Instruction::Impl::~Impl(void) throw()
{
}

void Instruction::Impl::out(std::ostream& os)
{
}

int Instruction::Impl::size(void) const
{
	return op_.size();
}

void Instruction::Impl::evaluate(op_proxy& proxy, SymTable& sym, int pos) const
{
	pos += size();
	proxy << op_.id();

	for(size_t i = 0 ; i < args_.size() ; ++i)
	{
		proxy << Evaluator::eval(op_[i], args_[i], sym, pos);
	}
}

// # ---------------------------------------------------------------------------

// # ===========================================================================

Instruction::Instruction(const OpTemplate& op, const arg_vec& args) : impl_(new Impl(op, args))
{
}

Instruction::~Instruction(void) throw()
{
	delete impl_;
}

void Instruction::out(std::ostream& os)
{
	impl_->out(os);
}

int Instruction::size(void) const
{
	return impl_->size();
}

const op_proxy Instruction::evaluate(SymTable& sym, int pos) const
{
	op_proxy p;
	impl_->evaluate(p, sym, pos);
	return p;
}

}}

