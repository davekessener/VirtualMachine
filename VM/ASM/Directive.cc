#include <iostream>
#include "Directive.h"
#include "SymTable.h"

namespace vm { namespace assembler {

Directive::Directive(void)
{
}

Directive::~Directive(void) throw()
{
}

void Directive::out(std::ostream& os)
{
}

int Directive::size(void) const
{
	return 0;
}

const op_proxy Directive::evaluate(SymTable& sym, int pos) const
{
	return op_proxy();
}

}}

