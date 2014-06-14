#ifndef VM_ASM_OPCODE_H
#define VM_ASM_OPCODE_H

#include "adef.h"
#include <iosfwd>

namespace vm
{
	namespace assembler
	{
		class Opcode
		{
			public:
				virtual ~Opcode( ) throw() { }
				virtual void out(std::ostream&) = 0;
			private:
		};

		inline std::ostream& operator<<(std::ostream& os, Opcode& op)
		{
			op.out(os);

			return os;
		}
	}
}

#endif

