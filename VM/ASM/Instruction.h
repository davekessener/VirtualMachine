#ifndef VM_ASM_INSTRUCTION_H
#define VM_ASM_INSTRUCTION_H

#include "adef.h"
#include "Opcode.h"

namespace vm
{
	namespace assembler
	{
		struct OpTemplate;

		class Instruction : public Opcode
		{
			typedef std::vector<std::string> arg_vec;

			public:
				Instruction(const OpTemplate&, const arg_vec&);
				virtual ~Instruction( ) throw();
				virtual void out(std::ostream&);
				virtual int size( ) const;
				virtual const op_proxy evaluate(SymTable&, int) const;
			private:
				struct Impl;
				Impl *impl_;
		};
	}
}

#endif

