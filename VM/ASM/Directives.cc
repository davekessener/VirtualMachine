#include <string>
#include "Directives.h"
#include "Directive.h"
#include "Opcode.h"
#include "Line.h"

namespace vm { namespace assembler { 
namespace Directives
{
	Opcode *process(const Line& line)
	{
		return new Directive;
	}
}
}}

