#include "Commands.h"
#include "CPU.h"
#include "Logger.h"

#define MXT_PC 15

namespace vm { namespace cpu {

namespace
{
	inline int fromUnsignedWordToSignedInt(WORD a)
	{
		return    (a & (1 << ((sizeof(WORD) << 3) - 1))) 
				? (~static_cast<int>(static_cast<WORD>(-1)) | a) 
				: a;
	}

	inline WORD getMemA(CPU& cpu)
	{
		WORD r = cpu.next();
		return cpu[r] + fromUnsignedWordToSignedInt(cpu.next());
	}
}

#define MXT_CMDO(id) void OP_##id::operator()(CPU& cpu) const

// # ===========================================================================

MXT_CMDO(hlt)
{
	cpu.halt();
}

MXT_CMDO(wait)
{
	cpu.suspend();
}

MXT_CMDO(log_x)
{
	WORD r = cpu.next();

	LOG("### > 0x%04x(%d)", cpu[r]);
}

MXT_CMDO(log_m)
{
	WORD r = cpu.next();
	int o = fromUnsignedWordToSignedInt(cpu.next());

	LOG("### > 0x%04x(%d)", cpu.RAM()[cpu[r] + o]);
}

// # ---------------------------------------------------------------------------

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
	int o = fromUnsignedWordToSignedInt(cpu.next());

	cpu[r1] = cpu.RAM()[cpu[r2] + o];
}

MXT_CMDO(ld_mc)
{
	WORD r = cpu.next();
	int o = fromUnsignedWordToSignedInt(cpu.next());
	WORD c = cpu.next();

	cpu.RAM()[cpu[r] + o] = c;
}

MXT_CMDO(ld_mx)
{
	WORD r1 = cpu.next();
	int o = fromUnsignedWordToSignedInt(cpu.next());
	WORD r2 = cpu.next();

	cpu.RAM()[cpu[r1] + o] = cpu[r2];
}

MXT_CMDO(ld_mm)
{
	WORD r1 = cpu.next();
	int o1 = fromUnsignedWordToSignedInt(cpu.next());
	WORD r2 = cpu.next();
	int o2 = fromUnsignedWordToSignedInt(cpu.next());

	cpu.RAM()[cpu[r1] + o1] = cpu.RAM()[cpu[r2] + o2];
}

// # ---------------------------------------------------------------------------

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

// # ---------------------------------------------------------------------------

MXT_CMDO(add_xc)
{
	WORD r = cpu.next();
	WORD c = cpu.next();

	cpu[r] += c;
}

MXT_CMDO(sub_xc)
{
	WORD r = cpu.next();
	WORD c = cpu.next();

	cpu[r] -= c;
}

MXT_CMDO(mul_xc)
{
	WORD r = cpu.next();
	WORD c = cpu.next();

	cpu[r] *= c;
}

MXT_CMDO(div_xc)
{
	WORD r = cpu.next();
	WORD c = cpu.next();

	cpu[r] /= c;
}

MXT_CMDO(mod_xc)
{
	WORD r = cpu.next();
	WORD c = cpu.next();

	cpu[r] %= c;
}

MXT_CMDO(and_xc)
{
	WORD r = cpu.next();
	WORD c = cpu.next();

	cpu[r] &= c;
}

MXT_CMDO(or_xc)
{
	WORD r = cpu.next();
	WORD c = cpu.next();

	cpu[r] |= c;
}

MXT_CMDO(xor_xc)
{
	WORD r = cpu.next();
	WORD c = cpu.next();

	cpu[r] ^= c;
}

// # ---------------------------------------------------------------------------

MXT_CMDO(add_mc)
{
	WORD r = cpu.next();
	int o = fromUnsignedWordToSignedInt(cpu.next());
	WORD c = cpu.next();

	cpu.RAM()[cpu[r] + o] += c;
}

MXT_CMDO(sub_mc)
{
	WORD r = cpu.next();
	int o = fromUnsignedWordToSignedInt(cpu.next());
	WORD c = cpu.next();

	cpu.RAM()[cpu[r] + o] -= c;
}

MXT_CMDO(mul_mc)
{
	WORD r = cpu.next();
	int o = fromUnsignedWordToSignedInt(cpu.next());
	WORD c = cpu.next();

	cpu.RAM()[cpu[r] + o] *= c;
}

MXT_CMDO(div_mc)
{
	WORD r = cpu.next();
	int o = fromUnsignedWordToSignedInt(cpu.next());
	WORD c = cpu.next();

	cpu.RAM()[cpu[r] + o] /= c;
}

MXT_CMDO(mod_mc)
{
	WORD r = cpu.next();
	int o = fromUnsignedWordToSignedInt(cpu.next());
	WORD c = cpu.next();

	cpu.RAM()[cpu[r] + o] %= c;
}

MXT_CMDO(and_mc)
{
	WORD r = cpu.next();
	int o = fromUnsignedWordToSignedInt(cpu.next());
	WORD c = cpu.next();

	cpu.RAM()[cpu[r] + o] &= c;
}

MXT_CMDO(or_mc)
{
	WORD r = cpu.next();
	int o = fromUnsignedWordToSignedInt(cpu.next());
	WORD c = cpu.next();

	cpu.RAM()[cpu[r] + o] |= c;
}

MXT_CMDO(xor_mc)
{
	WORD r = cpu.next();
	int o = fromUnsignedWordToSignedInt(cpu.next());
	WORD c = cpu.next();

	cpu.RAM()[cpu[r] + o] ^= c;
}

// # ---------------------------------------------------------------------------

MXT_CMDO(add_mx)
{
	WORD r1 = cpu.next();
	int o = fromUnsignedWordToSignedInt(cpu.next());
	WORD r2 = cpu.next();

	cpu.RAM()[cpu[r1] + o] += cpu[r2];
}

MXT_CMDO(sub_mx)
{
	WORD r1 = cpu.next();
	int o = fromUnsignedWordToSignedInt(cpu.next());
	WORD r2 = cpu.next();

	cpu.RAM()[cpu[r1] + o] -= cpu[r2];
}

MXT_CMDO(mul_mx)
{
	WORD r1 = cpu.next();
	int o = fromUnsignedWordToSignedInt(cpu.next());
	WORD r2 = cpu.next();

	cpu.RAM()[cpu[r1] + o] *= cpu[r2];
}

MXT_CMDO(div_mx)
{
	WORD r1 = cpu.next();
	int o = fromUnsignedWordToSignedInt(cpu.next());
	WORD r2 = cpu.next();

	cpu.RAM()[cpu[r1] + o] /= cpu[r2];
}

MXT_CMDO(mod_mx)
{
	WORD r1 = cpu.next();
	int o = fromUnsignedWordToSignedInt(cpu.next());
	WORD r2 = cpu.next();

	cpu.RAM()[cpu[r1] + o] %= cpu[r2];
}

MXT_CMDO(and_mx)
{
	WORD r1 = cpu.next();
	int o = fromUnsignedWordToSignedInt(cpu.next());
	WORD r2 = cpu.next();

	cpu.RAM()[cpu[r1] + o] &= cpu[r2];
}

MXT_CMDO(or_mx)
{
	WORD r1 = cpu.next();
	int o = fromUnsignedWordToSignedInt(cpu.next());
	WORD r2 = cpu.next();

	cpu.RAM()[cpu[r1] + o] |= cpu[r2];
}

MXT_CMDO(xor_mx)
{
	WORD r1 = cpu.next();
	int o = fromUnsignedWordToSignedInt(cpu.next());
	WORD r2 = cpu.next();

	cpu.RAM()[cpu[r1] + o] ^= cpu[r2];
}

// # ---------------------------------------------------------------------------

MXT_CMDO(add_xm)
{
	WORD r1 = cpu.next();
	WORD r2 = cpu.next();
	int o = fromUnsignedWordToSignedInt(cpu.next());

	cpu[r1] += cpu.RAM()[cpu[r2] + o];
}

MXT_CMDO(sub_xm)
{
	WORD r1 = cpu.next();
	WORD r2 = cpu.next();
	int o = fromUnsignedWordToSignedInt(cpu.next());

	cpu[r1] -= cpu.RAM()[cpu[r2] + o];
}

MXT_CMDO(mul_xm)
{
	WORD r1 = cpu.next();
	WORD r2 = cpu.next();
	int o = fromUnsignedWordToSignedInt(cpu.next());

	cpu[r1] *= cpu.RAM()[cpu[r2] + o];
}

MXT_CMDO(div_xm)
{
	WORD r1 = cpu.next();
	WORD r2 = cpu.next();
	int o = fromUnsignedWordToSignedInt(cpu.next());

	cpu[r1] /= cpu.RAM()[cpu[r2] + o];
}

MXT_CMDO(mod_xm)
{
	WORD r1 = cpu.next();
	WORD r2 = cpu.next();
	int o = fromUnsignedWordToSignedInt(cpu.next());

	cpu[r1] %= cpu.RAM()[cpu[r2] + o];
}

MXT_CMDO(and_xm)
{
	WORD r1 = cpu.next();
	WORD r2 = cpu.next();
	int o = fromUnsignedWordToSignedInt(cpu.next());

	cpu[r1] &= cpu.RAM()[cpu[r2] + o];
}

MXT_CMDO(or_xm)
{
	WORD r1 = cpu.next();
	WORD r2 = cpu.next();
	int o = fromUnsignedWordToSignedInt(cpu.next());

	cpu[r1] |= cpu.RAM()[cpu[r2] + o];
}

MXT_CMDO(xor_xm)
{
	WORD r1 = cpu.next();
	WORD r2 = cpu.next();
	int o = fromUnsignedWordToSignedInt(cpu.next());

	cpu[r1] ^= cpu.RAM()[cpu[r2] + o];
}

// # ---------------------------------------------------------------------------

MXT_CMDO(inc_m)
{
	WORD r = cpu.next();
	int o = fromUnsignedWordToSignedInt(cpu.next());

	++(cpu.RAM()[cpu[r] + o]);
}

MXT_CMDO(dec_m)
{
	WORD r = cpu.next();
	int o = fromUnsignedWordToSignedInt(cpu.next());

	--(cpu.RAM()[cpu[r] + o]);
}

MXT_CMDO(not_m)
{
	WORD r = cpu.next();
	int o = fromUnsignedWordToSignedInt(cpu.next());

	cpu.RAM()[cpu[r] + o] = ~cpu.RAM()[cpu[r] + o];
}

// # ---------------------------------------------------------------------------

MXT_CMDO(jmp_c)
{
	cpu[MXT_PC] = cpu.next();
}

MXT_CMDO(jmp_x)
{
	cpu[MXT_PC] = cpu[cpu.next()];
}

MXT_CMDO(jmp_m)
{
	WORD r = cpu.next();
	int o = fromUnsignedWordToSignedInt(cpu.next());
	cpu[MXT_PC] = cpu.RAM()[cpu[r] + o];
}

MXT_CMDO(jz_xc)
{
	WORD r = cpu.next();
	WORD a = cpu.next();

	if(!cpu[r])
	{
		cpu[MXT_PC] = a;
	}
}

MXT_CMDO(jz_xy)
{
	WORD r1 = cpu.next();
	WORD r2 = cpu.next();

	if(!cpu[r1])
	{
		cpu[MXT_PC] = cpu[r2];
	}
}

MXT_CMDO(jz_xm)
{
	WORD r1 = cpu.next();
	WORD r2 = cpu.next();
	int o = fromUnsignedWordToSignedInt(cpu.next());

	if(!cpu[r1])
	{
		cpu[MXT_PC] = cpu.RAM()[cpu[r2] + o];
	}
}

MXT_CMDO(jz_mc)
{
	WORD r = cpu.next();
	int o = fromUnsignedWordToSignedInt(cpu.next());
	WORD a = cpu.next();

	if(!cpu.RAM()[cpu[r] + o])
	{
		cpu[MXT_PC] = a;
	}
}

MXT_CMDO(jz_mx)
{
	WORD r1 = cpu.next();
	int o = fromUnsignedWordToSignedInt(cpu.next());
	WORD r2 = cpu.next();

	if(!cpu.RAM()[cpu[r1] + o])
	{
		cpu[MXT_PC] = cpu[r2];
	}
}

MXT_CMDO(jz_mm)
{
	WORD r1 = cpu.next();
	int o1 = fromUnsignedWordToSignedInt(cpu.next());
	WORD r2 = cpu.next();
	int o2 = fromUnsignedWordToSignedInt(cpu.next());

	if(!cpu.RAM()[cpu[r1] + o1])
	{
		cpu[MXT_PC] = cpu.RAM()[cpu[r2] + o2];
	}
}

MXT_CMDO(call_c)
{
	WORD a = cpu.next();
	cpu.push(cpu[MXT_PC]);
	cpu[MXT_PC] = a;
}

MXT_CMDO(call_x)
{
	WORD r = cpu.next();
	cpu.push(cpu[MXT_PC]);
	cpu[MXT_PC] = cpu[r];
}

MXT_CMDO(call_m)
{
	WORD r = cpu.next();
	int o = fromUnsignedWordToSignedInt(cpu.next());
	cpu.push(cpu[MXT_PC]);
	cpu[MXT_PC] = cpu.RAM()[cpu[r] + o];
}

MXT_CMDO(cllz_xc)
{
	WORD r = cpu.next();
	WORD a = cpu.next();

	if(!cpu[r])
	{
		cpu.push(cpu[MXT_PC]);
		cpu[MXT_PC] = a;
	}
}

MXT_CMDO(cllz_xy)
{
	WORD r1 = cpu.next();
	WORD r2 = cpu.next();

	if(!cpu[r1])
	{
		cpu.push(cpu[MXT_PC]);
		cpu[MXT_PC] = cpu[r2];
	}
}

MXT_CMDO(cllz_xm)
{
	WORD r1 = cpu.next();
	WORD r2 = cpu.next();
	int o = fromUnsignedWordToSignedInt(cpu.next());

	if(!cpu[r1])
	{
		cpu.push(cpu[MXT_PC]);
		cpu[MXT_PC] = cpu.RAM()[cpu[r2] + o];
	}
}

MXT_CMDO(cllz_mc)
{
	WORD r = cpu.next();
	int o = fromUnsignedWordToSignedInt(cpu.next());
	WORD a = cpu.next();

	if(!cpu.RAM()[cpu[r] + o])
	{
		cpu.push(cpu[MXT_PC]);
		cpu[MXT_PC] = a;
	}
}

MXT_CMDO(cllz_mx)
{
	WORD r1 = cpu.next();
	int o = fromUnsignedWordToSignedInt(cpu.next());
	WORD r2 = cpu.next();

	if(!cpu.RAM()[cpu[r1] + o])
	{
		cpu.push(cpu[MXT_PC]);
		cpu[MXT_PC] = cpu[r2];
	}
}

MXT_CMDO(cllz_mm)
{
	WORD r1 = cpu.next();
	int o1 = fromUnsignedWordToSignedInt(cpu.next());
	WORD r2 = cpu.next();
	int o2 = fromUnsignedWordToSignedInt(cpu.next());

	if(!cpu.RAM()[cpu[r1] + o1])
	{
		cpu.push(cpu[MXT_PC]);
		cpu[MXT_PC] = cpu.RAM()[cpu[r2] + o2];
	}
}

MXT_CMDO(ret)
{
	cpu[MXT_PC] = cpu.pop();
}

// # ---------------------------------------------------------------------------

MXT_CMDO(jr_c)
{
	cpu[MXT_PC] += fromUnsignedWordToSignedInt(cpu.next());
}

MXT_CMDO(jr_x)
{
	cpu[MXT_PC] += fromUnsignedWordToSignedInt(cpu[cpu.next()]);
}

MXT_CMDO(jr_m)
{
	WORD r = cpu.next();
	int o = fromUnsignedWordToSignedInt(cpu.next());
	cpu[MXT_PC] += fromUnsignedWordToSignedInt(cpu.RAM()[cpu[r] + o]);
}

MXT_CMDO(jrz_xc)
{
	WORD r = cpu.next();
	WORD a = cpu.next();

	if(!cpu[r])
	{
		cpu[MXT_PC] += fromUnsignedWordToSignedInt(a);
	}
}

MXT_CMDO(jrz_xy)
{
	WORD r1 = cpu.next();
	WORD r2 = cpu.next();

	if(!cpu[r1])
	{
		cpu[MXT_PC] += fromUnsignedWordToSignedInt(cpu[r2]);
	}
}

MXT_CMDO(jrz_xm)
{
	WORD r1 = cpu.next();
	WORD r2 = cpu.next();
	int o = fromUnsignedWordToSignedInt(cpu.next());

	if(!cpu[r1])
	{
		cpu[MXT_PC] += fromUnsignedWordToSignedInt(cpu.RAM()[cpu[r2] + o]);
	}
}

MXT_CMDO(jrz_mc)
{
	WORD r = cpu.next();
	int o = fromUnsignedWordToSignedInt(cpu.next());
	WORD a = cpu.next();

	if(!cpu.RAM()[cpu[r] + o])
	{
		cpu[MXT_PC] += fromUnsignedWordToSignedInt(a);
	}
}

MXT_CMDO(jrz_mx)
{
	WORD r1 = cpu.next();
	int o = fromUnsignedWordToSignedInt(cpu.next());
	WORD r2 = cpu.next();

	if(!cpu.RAM()[cpu[r1] + o])
	{
		cpu[MXT_PC] += fromUnsignedWordToSignedInt(cpu[r2]);
	}
}

MXT_CMDO(jrz_mm)
{
	WORD r1 = cpu.next();
	int o1 = fromUnsignedWordToSignedInt(cpu.next());
	WORD r2 = cpu.next();
	int o2 = fromUnsignedWordToSignedInt(cpu.next());

	if(!cpu.RAM()[cpu[r1] + o1])
	{
		cpu[MXT_PC] += fromUnsignedWordToSignedInt(cpu.RAM()[cpu[r2] + o2]);
	}
}

MXT_CMDO(cllr_c)
{
	WORD a = cpu.next();
	cpu.push(cpu[MXT_PC]);
	cpu[MXT_PC] += fromUnsignedWordToSignedInt(a);
}

MXT_CMDO(cllr_x)
{
	WORD r = cpu.next();
	cpu.push(cpu[MXT_PC]);
	cpu[MXT_PC] += fromUnsignedWordToSignedInt(cpu[r]);
}

MXT_CMDO(cllr_m)
{
	WORD r = cpu.next();
	int o = fromUnsignedWordToSignedInt(cpu.next());
	cpu.push(cpu[MXT_PC]);
	cpu[MXT_PC] += fromUnsignedWordToSignedInt(cpu.RAM()[cpu[r] + o]);
}

MXT_CMDO(clrz_xc)
{
	WORD r = cpu.next();
	WORD a = cpu.next();

	if(!cpu[r])
	{
		cpu.push(cpu[MXT_PC]);
		cpu[MXT_PC] += fromUnsignedWordToSignedInt(a);
	}
}

MXT_CMDO(clrz_xy)
{
	WORD r1 = cpu.next();
	WORD r2 = cpu.next();

	if(!cpu[r1])
	{
		cpu.push(cpu[MXT_PC]);
		cpu[MXT_PC] += fromUnsignedWordToSignedInt(cpu[r2]);
	}
}

MXT_CMDO(clrz_xm)
{
	WORD r1 = cpu.next();
	WORD r2 = cpu.next();
	int o = fromUnsignedWordToSignedInt(cpu.next());

	if(!cpu[r1])
	{
		cpu.push(cpu[MXT_PC]);
		cpu[MXT_PC] += fromUnsignedWordToSignedInt(cpu.RAM()[cpu[r2] + o]);
	}
}

MXT_CMDO(clrz_mc)
{
	WORD r = cpu.next();
	int o = fromUnsignedWordToSignedInt(cpu.next());
	WORD a = cpu.next();

	if(!cpu.RAM()[cpu[r] + o])
	{
		cpu.push(cpu[MXT_PC]);
		cpu[MXT_PC] += fromUnsignedWordToSignedInt(a);
	}
}

MXT_CMDO(clrz_mx)
{
	WORD r1 = cpu.next();
	int o = fromUnsignedWordToSignedInt(cpu.next());
	WORD r2 = cpu.next();

	if(!cpu.RAM()[cpu[r1] + o])
	{
		cpu.push(cpu[MXT_PC]);
		cpu[MXT_PC] += fromUnsignedWordToSignedInt(cpu[r2]);
	}
}

MXT_CMDO(clrz_mm)
{
	WORD r1 = cpu.next();
	int o1 = fromUnsignedWordToSignedInt(cpu.next());
	WORD r2 = cpu.next();
	int o2 = fromUnsignedWordToSignedInt(cpu.next());

	if(!cpu.RAM()[cpu[r1] + o1])
	{
		cpu.push(cpu[MXT_PC]);
		cpu[MXT_PC] += fromUnsignedWordToSignedInt(cpu.RAM()[cpu[r2] + o2]);
	}
}

// # ---------------------------------------------------------------------------

MXT_CMDO(push_c)
{
	WORD a = cpu.next();
	cpu.push(a);
}

MXT_CMDO(push_x)
{
	WORD r = cpu.next();
	cpu.push(cpu[r]);
}

MXT_CMDO(push_m)
{
	WORD r = cpu.next();
	int o = fromUnsignedWordToSignedInt(cpu.next());
	cpu.push(cpu.RAM()[cpu[r] + o]);
}

MXT_CMDO(pop_x)
{
	WORD r = cpu.next();
	cpu[r] = cpu.pop();
}

MXT_CMDO(pop_m)
{
	WORD r = cpu.next();
	int o = fromUnsignedWordToSignedInt(cpu.next());
	cpu.RAM()[cpu[r] + o] = cpu.pop();
}

// # ---------------------------------------------------------------------------

MXT_CMDO(in_cx)
{
	WORD i = cpu.next();
	WORD r = cpu.next();

	cpu[r] = cpu.out(i);
}

MXT_CMDO(in_cm)
{
	WORD i = cpu.next();
	WORD r = cpu.next();
	int o = fromUnsignedWordToSignedInt(cpu.next());

	cpu.RAM()[cpu[r] + o] = cpu.out(i);
}

MXT_CMDO(in_xy)
{
	WORD r1 = cpu.next();
	WORD r2 = cpu.next();

	cpu[r2] = cpu.out(cpu[r1]);
}

MXT_CMDO(in_xm)
{
	WORD r1 = cpu.next();
	WORD r2 = cpu.next();
	int o = fromUnsignedWordToSignedInt(cpu.next());

	cpu.RAM()[cpu[r2] + o] = cpu.out(cpu[r1]);
}

MXT_CMDO(in_mx)
{
	WORD r1 = cpu.next();
	int o = fromUnsignedWordToSignedInt(cpu.next());
	WORD r2 = cpu.next();

	cpu[r2] = cpu.out(cpu.RAM()[cpu[r1] + o]);
}

MXT_CMDO(in_mm)
{
	WORD r1 = cpu.next();
	int o1 = fromUnsignedWordToSignedInt(cpu.next());
	WORD r2 = cpu.next();
	int o2 = fromUnsignedWordToSignedInt(cpu.next());

	cpu.RAM()[cpu[r2] + o2] = cpu.out(cpu.RAM()[cpu[r1] + o1]);
}

MXT_CMDO(out_cc)
{
	WORD i = cpu.next();
	WORD c = cpu.next();

	cpu.in(i, c);
}

MXT_CMDO(out_cx)
{
	WORD i = cpu.next();
	WORD r = cpu.next();

	cpu.in(i, cpu[r]);
}

MXT_CMDO(out_cm)
{
	WORD i = cpu.next();
	WORD r = cpu.next();
	int o = fromUnsignedWordToSignedInt(cpu.next());

	cpu.in(i, cpu.RAM()[cpu[r] + o]);
}

MXT_CMDO(out_xc)
{
	WORD r = cpu.next();
	WORD i = cpu.next();

	cpu.in(cpu[r], i);
}

MXT_CMDO(out_xy)
{
	WORD r1 = cpu.next();
	WORD r2 = cpu.next();

	cpu.in(cpu[r1], cpu[r2]);
}

MXT_CMDO(out_xm)
{
	WORD r1 = cpu.next();
	WORD r2 = cpu.next();
	int o = fromUnsignedWordToSignedInt(cpu.next());

	cpu.in(cpu[r1], cpu.RAM()[cpu[r2] + o]);
}

MXT_CMDO(out_mc)
{
	WORD r = cpu.next();
	int o = fromUnsignedWordToSignedInt(cpu.next());
	WORD i = cpu.next();

	cpu.in(cpu.RAM()[cpu[r] + o], i);
}

MXT_CMDO(out_mx)
{
	WORD r1 = cpu.next();
	int o = fromUnsignedWordToSignedInt(cpu.next());
	WORD r2 = cpu.next();

	cpu.in(cpu.RAM()[cpu[r1] + o], cpu[r2]);
}

MXT_CMDO(out_mm)
{
	WORD r1 = cpu.next();
	int o1 = fromUnsignedWordToSignedInt(cpu.next());
	WORD r2 = cpu.next();
	int o2 = fromUnsignedWordToSignedInt(cpu.next());

	cpu.in(cpu.RAM()[cpu[r1] + o1], cpu.RAM()[cpu[r2] + o2]);
}

// # ---------------------------------------------------------------------------

MXT_CMDO(ei)
{
	cpu.enableInterrupt(true);
}

MXT_CMDO(di)
{
	cpu.enableInterrupt(false);
}

// # ===========================================================================

#undef MXT_CMDO

}}

