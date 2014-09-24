#ifndef VM_ASM_EVALUATOR_H
#define VM_ASM_EVALUATOR_H

#include <string>
#include <vector>
#include "adef.h"

namespace vm
{
	namespace assembler
	{
		struct SymTable;

		namespace Evaluator
		{
			typedef std::vector<WORD> eval_t;
			eval_t eval(Parameter, const std::string&, const SymTable&, int);
			Parameter deduceParameter(const std::string&);
		}
	}
}

#endif

