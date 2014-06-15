#ifndef VM_ASM_EVALUATOR_H
#define VM_ASM_EVALUATOR_H

#include <string>
#include "adef.h"

namespace vm
{
	namespace assembler
	{
		struct SymTable;

		namespace Evaluator
		{
			WORD eval(Parameter, const std::string&, SymTable&, int);
			Parameter deduceParameter(const std::string&);
		}
	}
}

#endif

