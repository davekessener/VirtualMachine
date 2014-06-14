#ifndef VM_ASM_INSTRUCTION_H
#define VM_ASM_INSTRUCTION_H

#include "adef.h"
#include "Opcode.h"

namespace vm
{
	namespace assembler
	{
		class Instruction : public Opcode
		{
			public:
				Instruction( );
				virtual ~Instruction( ) throw();
				virtual void out(std::ostream&);
			private:
				struct Impl;
				Impl *impl_;
		};
	}
}

#endif

