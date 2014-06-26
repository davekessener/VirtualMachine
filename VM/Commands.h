#ifndef VM_COMMANDS_H
#define VM_COMMANDS_H

#include "ddef.h"
#include "Instruction.h"

#define MXT_hlt_NAME "hlt"
#define MXT_ld_xc_NAME "ld"

namespace vm
{
	namespace cpu
	{
#define MXT_CMD(id,n,...) \
class OP_##id : public Instruction \
{\
public:\
OP_##id( ) : Instruction(n, {__VA_ARGS__}) { }\
void operator()(CPU&) const;\
}

// # ===========================================================================
		MXT_CMD(hlt,     "hlt");
		MXT_CMD(wait,    "wait");
		MXT_CMD(log_x,	 "log",  Arguments::REGISTER);
		MXT_CMD(log_m,	 "log",  Arguments::MEMORY);
// # ---------------------------------------------------------------------------
		MXT_CMD(mcpy_xyz,	 "mcpy", Arguments::REGISTER, Arguments::REGISTER, Arguments::REGISTER);
		MXT_CMD(mcpy_xyc,	 "mcpy", Arguments::REGISTER, Arguments::REGISTER, Arguments::CONSTANT);
		MXT_CMD(mcpy_xcy,	 "mcpy", Arguments::REGISTER, Arguments::CONSTANT, Arguments::REGISTER);
		MXT_CMD(mcpy_cxy,	 "mcpy", Arguments::CONSTANT, Arguments::REGISTER, Arguments::REGISTER);
		MXT_CMD(mcpy_xcc,	 "mcpy", Arguments::REGISTER, Arguments::CONSTANT, Arguments::CONSTANT);
		MXT_CMD(mcpy_cxc,	 "mcpy", Arguments::CONSTANT, Arguments::REGISTER, Arguments::CONSTANT);
		MXT_CMD(mcpy_ccx,	 "mcpy", Arguments::CONSTANT, Arguments::CONSTANT, Arguments::REGISTER);
		MXT_CMD(mcpy_ccc,	 "mcpy", Arguments::CONSTANT, Arguments::CONSTANT, Arguments::CONSTANT);
// # ---------------------------------------------------------------------------
		MXT_CMD(ld_xc,   "ld",   Arguments::REGISTER, Arguments::CONSTANT);
		MXT_CMD(ld_xy,   "ld",   Arguments::REGISTER, Arguments::REGISTER);
		MXT_CMD(ld_xm,   "ld",   Arguments::REGISTER, Arguments::MEMORY);
		MXT_CMD(ld_mc,   "ld",   Arguments::MEMORY,   Arguments::CONSTANT);
		MXT_CMD(ld_mx,   "ld",   Arguments::MEMORY,   Arguments::REGISTER);
		MXT_CMD(ld_mm,   "ld",   Arguments::MEMORY,   Arguments::MEMORY);
// # ---------------------------------------------------------------------------
		MXT_CMD(inc_x,   "inc",  Arguments::REGISTER);
		MXT_CMD(dec_x,   "dec",  Arguments::REGISTER);
		MXT_CMD(add_xy,  "add",  Arguments::REGISTER, Arguments::REGISTER);
		MXT_CMD(sub_xy,  "sub",  Arguments::REGISTER, Arguments::REGISTER);
		MXT_CMD(mul_xy,  "mul",  Arguments::REGISTER, Arguments::REGISTER);
		MXT_CMD(div_xy,  "div",  Arguments::REGISTER, Arguments::REGISTER);
		MXT_CMD(mod_xy,  "mod",  Arguments::REGISTER, Arguments::REGISTER);
		MXT_CMD(and_xy,  "and",  Arguments::REGISTER, Arguments::REGISTER);
		MXT_CMD(or_xy,   "or",   Arguments::REGISTER, Arguments::REGISTER);
		MXT_CMD(xor_xy,  "xor",  Arguments::REGISTER, Arguments::REGISTER);
		MXT_CMD(not_x,   "not",  Arguments::REGISTER);
// # ---------------------------------------------------------------------------
		MXT_CMD(add_xc,  "add",  Arguments::REGISTER, Arguments::CONSTANT);
		MXT_CMD(sub_xc,  "sub",  Arguments::REGISTER, Arguments::CONSTANT);
		MXT_CMD(mul_xc,  "mul",  Arguments::REGISTER, Arguments::CONSTANT);
		MXT_CMD(div_xc,  "div",  Arguments::REGISTER, Arguments::CONSTANT);
		MXT_CMD(mod_xc,  "mod",  Arguments::REGISTER, Arguments::CONSTANT);
		MXT_CMD(and_xc,  "and",  Arguments::REGISTER, Arguments::CONSTANT);
		MXT_CMD(or_xc,   "or",   Arguments::REGISTER, Arguments::CONSTANT);
		MXT_CMD(xor_xc,  "xor",  Arguments::REGISTER, Arguments::CONSTANT);
// # ---------------------------------------------------------------------------
		MXT_CMD(add_mc,  "add",  Arguments::MEMORY,   Arguments::CONSTANT);
		MXT_CMD(sub_mc,  "sub",  Arguments::MEMORY,   Arguments::CONSTANT);
		MXT_CMD(mul_mc,  "mul",  Arguments::MEMORY,   Arguments::CONSTANT);
		MXT_CMD(div_mc,  "div",  Arguments::MEMORY,   Arguments::CONSTANT);
		MXT_CMD(mod_mc,  "mod",  Arguments::MEMORY,   Arguments::CONSTANT);
		MXT_CMD(and_mc,  "and",  Arguments::MEMORY,   Arguments::CONSTANT);
		MXT_CMD(or_mc,   "or",   Arguments::MEMORY,   Arguments::CONSTANT);
		MXT_CMD(xor_mc,  "xor",  Arguments::MEMORY,   Arguments::CONSTANT);
// # ---------------------------------------------------------------------------
		MXT_CMD(add_mx,  "add",  Arguments::MEMORY,   Arguments::REGISTER);
		MXT_CMD(sub_mx,  "sub",  Arguments::MEMORY,   Arguments::REGISTER);
		MXT_CMD(mul_mx,  "mul",  Arguments::MEMORY,   Arguments::REGISTER);
		MXT_CMD(div_mx,  "div",  Arguments::MEMORY,   Arguments::REGISTER);
		MXT_CMD(mod_mx,  "mod",  Arguments::MEMORY,   Arguments::REGISTER);
		MXT_CMD(and_mx,  "and",  Arguments::MEMORY,   Arguments::REGISTER);
		MXT_CMD(or_mx,   "or",   Arguments::MEMORY,   Arguments::REGISTER);
		MXT_CMD(xor_mx,  "xor",  Arguments::MEMORY,   Arguments::REGISTER);
// # ---------------------------------------------------------------------------
		MXT_CMD(add_xm,  "add",  Arguments::REGISTER, Arguments::MEMORY);
		MXT_CMD(sub_xm,  "sub",  Arguments::REGISTER, Arguments::MEMORY);
		MXT_CMD(mul_xm,  "mul",  Arguments::REGISTER, Arguments::MEMORY);
		MXT_CMD(div_xm,  "div",  Arguments::REGISTER, Arguments::MEMORY);
		MXT_CMD(mod_xm,  "mod",  Arguments::REGISTER, Arguments::MEMORY);
		MXT_CMD(and_xm,  "and",  Arguments::REGISTER, Arguments::MEMORY);
		MXT_CMD(or_xm,   "or",   Arguments::REGISTER, Arguments::MEMORY);
		MXT_CMD(xor_xm,  "xor",  Arguments::REGISTER, Arguments::MEMORY);
// # ---------------------------------------------------------------------------
		MXT_CMD(inc_m,   "inc",  Arguments::MEMORY);
		MXT_CMD(dec_m,   "dec",  Arguments::MEMORY);
		MXT_CMD(not_m,   "not",  Arguments::MEMORY);
// # ---------------------------------------------------------------------------
		MXT_CMD(jmp_c,   "jmp",  Arguments::CONSTANT);
		MXT_CMD(jmp_x,   "jmp",  Arguments::REGISTER);
		MXT_CMD(jmp_m,   "jmp",  Arguments::MEMORY);
		MXT_CMD(jz_xc,   "jz",   Arguments::REGISTER, Arguments::CONSTANT);
		MXT_CMD(jz_xy,   "jz",   Arguments::REGISTER, Arguments::REGISTER);
		MXT_CMD(jz_xm,   "jz",   Arguments::REGISTER, Arguments::MEMORY);
		MXT_CMD(jz_mc,   "jz",   Arguments::MEMORY,   Arguments::CONSTANT);
		MXT_CMD(jz_mx,   "jz",   Arguments::MEMORY,   Arguments::REGISTER);
		MXT_CMD(jz_mm,   "jz",   Arguments::MEMORY,   Arguments::MEMORY);
		MXT_CMD(call_c,  "call", Arguments::CONSTANT);
		MXT_CMD(call_x,  "call", Arguments::REGISTER);
		MXT_CMD(call_m,  "call", Arguments::MEMORY);
		MXT_CMD(cllz_xc, "cllz", Arguments::REGISTER, Arguments::CONSTANT);
		MXT_CMD(cllz_xy, "cllz", Arguments::REGISTER, Arguments::REGISTER);
		MXT_CMD(cllz_xm, "cllz", Arguments::REGISTER, Arguments::MEMORY);
		MXT_CMD(cllz_mc, "cllz", Arguments::MEMORY,   Arguments::CONSTANT);
		MXT_CMD(cllz_mx, "cllz", Arguments::MEMORY,   Arguments::REGISTER);
		MXT_CMD(cllz_mm, "cllz", Arguments::MEMORY,   Arguments::MEMORY);
		MXT_CMD(ret,     "ret");
// # ---------------------------------------------------------------------------
		MXT_CMD(jr_c,    "jr",   Arguments::CONSTANT);
		MXT_CMD(jr_x,    "jr",   Arguments::REGISTER);
		MXT_CMD(jr_m,    "jr",   Arguments::MEMORY);
		MXT_CMD(jrz_xc,  "jrz",  Arguments::REGISTER, Arguments::CONSTANT);
		MXT_CMD(jrz_xy,  "jrz",  Arguments::REGISTER, Arguments::REGISTER);
		MXT_CMD(jrz_xm,  "jrz",  Arguments::REGISTER, Arguments::MEMORY);
		MXT_CMD(jrz_mc,  "jrz",  Arguments::MEMORY,   Arguments::CONSTANT);
		MXT_CMD(jrz_mx,  "jrz",  Arguments::MEMORY,   Arguments::REGISTER);
		MXT_CMD(jrz_mm,  "jrz",  Arguments::MEMORY,   Arguments::MEMORY);
		MXT_CMD(cllr_c,  "cllr", Arguments::CONSTANT);
		MXT_CMD(cllr_x,  "cllr", Arguments::REGISTER);
		MXT_CMD(cllr_m,  "cllr", Arguments::MEMORY);
		MXT_CMD(clrz_xc, "clrz", Arguments::REGISTER, Arguments::CONSTANT);
		MXT_CMD(clrz_xy, "clrz", Arguments::REGISTER, Arguments::REGISTER);
		MXT_CMD(clrz_xm, "clrz", Arguments::REGISTER, Arguments::MEMORY);
		MXT_CMD(clrz_mc, "clrz", Arguments::MEMORY,   Arguments::CONSTANT);
		MXT_CMD(clrz_mx, "clrz", Arguments::MEMORY,   Arguments::REGISTER);
		MXT_CMD(clrz_mm, "clrz", Arguments::MEMORY,   Arguments::MEMORY);
// # ---------------------------------------------------------------------------
		MXT_CMD(push_c,  "push", Arguments::CONSTANT);
		MXT_CMD(push_x,  "push", Arguments::REGISTER);
		MXT_CMD(push_m,  "push", Arguments::MEMORY);
		MXT_CMD(pop_x,   "pop",  Arguments::REGISTER);
		MXT_CMD(pop_m,   "pop",  Arguments::MEMORY);
// # ---------------------------------------------------------------------------
		MXT_CMD(in_cx,   "in",   Arguments::CONSTANT, Arguments::REGISTER);
		MXT_CMD(in_cm,   "in",   Arguments::CONSTANT, Arguments::MEMORY);
		MXT_CMD(in_xy,   "in",   Arguments::REGISTER, Arguments::REGISTER);
		MXT_CMD(in_xm,   "in",   Arguments::REGISTER, Arguments::MEMORY);
		MXT_CMD(in_mx,   "in",   Arguments::MEMORY,   Arguments::REGISTER);
		MXT_CMD(in_mm,   "in",   Arguments::MEMORY,   Arguments::MEMORY);
		MXT_CMD(out_cc,  "out",  Arguments::CONSTANT, Arguments::CONSTANT);
		MXT_CMD(out_cx,  "out",  Arguments::CONSTANT, Arguments::REGISTER);
		MXT_CMD(out_cm,  "out",  Arguments::CONSTANT, Arguments::MEMORY);
		MXT_CMD(out_xc,  "out",  Arguments::REGISTER, Arguments::CONSTANT);
		MXT_CMD(out_xy,  "out",  Arguments::REGISTER, Arguments::REGISTER);
		MXT_CMD(out_xm,  "out",  Arguments::REGISTER, Arguments::MEMORY);
		MXT_CMD(out_mc,  "out",  Arguments::MEMORY,   Arguments::CONSTANT);
		MXT_CMD(out_mx,  "out",  Arguments::MEMORY,   Arguments::REGISTER);
		MXT_CMD(out_mm,  "out",  Arguments::MEMORY,   Arguments::MEMORY);
// # ---------------------------------------------------------------------------
		MXT_CMD(ei,      "ei");
		MXT_CMD(di,      "di");
// # ===========================================================================

#undef MXT_CMD
	}
}

#endif

