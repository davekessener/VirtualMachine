#include <string>
#include "Directives.h"
#include "Directive.h"
#include "Opcode.h"

namespace vm { namespace assembler { 
namespace Directives
{
	Opcode *process(const std::string& line)
	{
		return new Directive;
	}
}
}}

