#include <iostream>
#include "Directive.h"
#include "SymTable.h"
#include "Evaluator.h"

namespace vm { namespace assembler {

DirectiveDB::DirectiveDB(const std::string& line) : expr_(line)
{
}

DirectiveDB::~DirectiveDB(void) throw()
{
}

void DirectiveDB::out(std::ostream& os)
{
}

int DirectiveDB::size(void) const
{
	return 1;
}

const op_proxy DirectiveDB::evaluate(SymTable& sym, int pos) const
{
	op_proxy pr;

	pr << Evaluator::eval(Parameter::CONSTANT, expr_, sym, pos);

	return pr;
}

}}

