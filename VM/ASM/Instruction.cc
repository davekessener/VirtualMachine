#include <iostream>
#include "Instruction.h"

namespace vm { namespace assembler {

class Instruction::Impl
{
	public:
		Impl( );
		~Impl( ) throw();
		inline void out(std::ostream&);
	private:
};

// # ===========================================================================

Instruction::Impl::Impl(void)
{
}

Instruction::Impl::~Impl(void) throw()
{
}

inline void Instruction::Impl::out(std::ostream& os)
{
}

// # ---------------------------------------------------------------------------

// # ===========================================================================

Instruction::Instruction(void) : impl_(new Impl)
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

}}

