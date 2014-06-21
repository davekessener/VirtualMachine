#include <iostream>
#include "Directive.h"
#include "SymTable.h"
#include "Evaluator.h"
#include "Logger.h"

namespace vm { namespace assembler {

DirectiveDB::DirectiveDB(const std::string& line) : expr_(line)
{
}

void DirectiveDB::out(std::ostream& os)
{
}

const op_proxy DirectiveDB::evaluate(SymTable& sym, int pos) const
{
	op_proxy pr;

	pr << Evaluator::eval(Parameter::CONSTANT, expr_, sym, pos);

	return pr;
}

// # ===========================================================================

DirectiveORG::DirectiveORG(WORD w) : off_(w)
{
//	LOG("[ORG] New offset: 0x%04x", off_);
}

void DirectiveORG::out(std::ostream& os)
{
}

const op_proxy DirectiveORG::evaluate(SymTable& sym, int pos) const
{
//	sym.setOffset(off_ - pos);
//
//	LOG("[ORG] Setting offset @0x%04x to %d", pos, off_ - pos);

	return op_proxy();
}

}}

