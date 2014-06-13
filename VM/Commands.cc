#include "Commands.h"
#include "CPU.h"

namespace vm { namespace cpu {

#define MXT_CMDO(id) void OP_##id::operator()(CPU& cpu) const

MXT_CMDO(hlt)
{
	cpu.halt();
}

MXT_CMDO(ld_xc)
{
	WORD r = cpu.next();
	WORD c = cpu.next();

	cpu[r] = c;
}

MXT_CMDO(ld_xy)
{
	WORD r1 = cpu.next();
	WORD r2 = cpu.next();

	cpu[r1] = cpu[r2];
}

MXT_CMDO(ld_xm)
{
	WORD r1 = cpu.next();
	WORD r2 = cpu.next();

	cpu.read(cpu[r2], 1, &cpu[r1]);
}

MXT_CMDO(ld_mx)
{
	WORD r1 = cpu.next();
	WORD r2 = cpu.next();

	cpu.write(cpu[r1], 1, &cpu[r2]);
}

MXT_CMDO(inc_x)
{
	WORD r = cpu.next();

	++cpu[r];
}

MXT_CMDO(dec_x)
{
	WORD r = cpu.next();

	--cpu[r];
}

MXT_CMDO(add_xy)
{
	WORD r1 = cpu.next();
	WORD r2 = cpu.next();

	cpu[r1] += cpu[r2];
}

MXT_CMDO(sub_xy)
{
	WORD r1 = cpu.next();
	WORD r2 = cpu.next();

	cpu[r1] -= cpu[r2];
}

MXT_CMDO(mul_xy)
{
	WORD r1 = cpu.next();
	WORD r2 = cpu.next();

	cpu[r1] *= cpu[r2];
}

MXT_CMDO(div_xy)
{
	WORD r1 = cpu.next();
	WORD r2 = cpu.next();

	cpu[r1] /= cpu[r2];
}

MXT_CMDO(mod_xy)
{
	WORD r1 = cpu.next();
	WORD r2 = cpu.next();

	cpu[r1] %= cpu[r2];
}

MXT_CMDO(and_xy)
{
	WORD r1 = cpu.next();
	WORD r2 = cpu.next();

	cpu[r1] &= cpu[r2];
}

MXT_CMDO(or_xy)
{
	WORD r1 = cpu.next();
	WORD r2 = cpu.next();

	cpu[r1] |= cpu[r2];
}

MXT_CMDO(xor_xy)
{
	WORD r1 = cpu.next();
	WORD r2 = cpu.next();

	cpu[r1] ^= cpu[r2];
}

MXT_CMDO(not_x)
{
	WORD r = cpu.next();

	cpu[r] = ~cpu[r];
}

MXT_CMDO(jmp_c)
{
	cpu[0] = cpu.next();
}

MXT_CMDO(jz_xc)
{
	WORD r = cpu.next();
	WORD a = cpu.next();

	if(!cpu[r])
	{
		cpu[0] = a;
	}
}

MXT_CMDO(call_c)
{
	WORD a = cpu.next();
	cpu.push(cpu[0]);
	cpu[0] = a;
}

MXT_CMDO(cllz_xc)
{
	WORD r = cpu.next();
	WORD a = cpu.next();

	if(!cpu[r])
	{
		cpu.push(cpu[0]);
		cpu[0] = a;
	}
}

MXT_CMDO(ret)
{
	cpu[0] = cpu.pop();
}

MXT_CMDO(push_x)
{
	WORD r = cpu.next();
	cpu.push(cpu[r]);
}

MXT_CMDO(pop_x)
{
	WORD r = cpu.next();
	cpu[r] = cpu.pop();
}

MXT_CMDO(in_cx)
{
	WORD i = cpu.next();
	WORD r = cpu.next();

	cpu[r] = cpu.out(i);
}

MXT_CMDO(out_cx)
{
	WORD i = cpu.next();
	WORD r = cpu.next();

	cpu.in(i, cpu[r]);
}

MXT_CMDO(ei)
{
	cpu.enableInterrupt(true);
}

MXT_CMDO(di)
{
	cpu.enableInterrupt(false);
}

#undef MXT_CMDO

}}

