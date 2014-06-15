#include "Marker.h"
#include "SymTable.h"

namespace vm { namespace assembler {

Marker::Marker(const std::string& n) : name_(n)
{
}

Marker::~Marker(void) throw()
{
}

void Marker::out(std::ostream& os)
{
}

int Marker::size(void) const
{
	return 0;
}

const op_proxy Marker::evaluate(SymTable& sym, int pos) const
{
	return op_proxy();
}

}}

