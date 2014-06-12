#include "Commands.h"
#include "CPU.h"

namespace vm { namespace cpu {

#define MXT_CMDO(id) bool OP_##id::operator()(CPU& cpu) const

MXT_CMDO(hlt)
{
	return false;
}

MXT_CMDO(ld_xc)
{
	WORD r = cpu.next();
	WORD c = cpu.next();

	cpu[r] = c;

	return true;
}

MXT_CMDO(ld_xy)
{
	WORD r1 = cpu.next();
	WORD r2 = cpu.next();

	cpu[r1] = cpu[r2];

	return true;
}

#undef MXT_CMDO

}}

