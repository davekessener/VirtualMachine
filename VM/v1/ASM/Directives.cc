#include <string>
#include "Directives.h"
#include "Directive.h"
#include "Opcode.h"
#include "Line.h"
#include "Token.h"
#include "Evaluator.h"
#include "SymTable.h"
#include "Logger.h"
#include "ASMException.h"
#include "stringtools.h"

#define MXT_PD_DB ".db"
#define MXT_PD_ORG ".org"

namespace vm { namespace assembler { 

namespace
{
	const std::string join(const std::vector<Token>&);
}

namespace Directives
{
	Opcode *process(const Line& line)
	{
		std::string id(line[0].str());
		std::vector<Token> v(line.cbegin() + 1, line.cend());

		if(id == MXT_PD_DB)
		{
			return new DirectiveDB(join(v));
		}
		else if(id == MXT_PD_ORG)
		{
			return new DirectiveORG(Evaluator::eval(Parameter::CONSTANT, join(v), SymTable(), 0).at(0));
		}
		else
		{
			MXT_LOGANDTHROW_T(line[0], "ERR: Unknown directive '%s'", line[0].str().c_str());
		}
	}
}

namespace
{
	const std::string join(const std::vector<Token>& t)
	{
		std::string s;
		for(const Token& tt : t) s += tt.str();
		return s;
	}
}

}}

