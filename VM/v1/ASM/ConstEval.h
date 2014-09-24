#ifndef VM_ASM_CONSTEVAL_H
#define VM_ASM_CONSTEVAL_H

#include <string>
#include "adef.h"

namespace vm
{
	namespace assembler
	{
		struct SymTable;

		namespace Evaluator
		{
			WORD consteval(const std::string&, const SymTable&, int);
		}
	}
}

#endif

