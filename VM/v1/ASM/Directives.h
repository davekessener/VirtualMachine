#ifndef VM_ASM_DIRECTIVES_H
#define VM_ASM_DIRECTIVES_H

#include "adef.h"

namespace vm
{
	namespace assembler
	{
		struct Opcode;
		struct Line;

		namespace Directives
		{
			Opcode *process(const Line&);
		}
	}
}

#endif

