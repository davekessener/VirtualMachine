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

		MXT_CMD(hlt,     "hlt");
		MXT_CMD(ld_xc,   "ld",   Arguments::REGISTER, Arguments::CONSTANT);
		MXT_CMD(ld_xy,   "ld",   Arguments::REGISTER, Arguments::REGISTER);
		MXT_CMD(ld_xm,   "ld",   Arguments::REGISTER, Arguments::MEMORY);
		MXT_CMD(ld_mx,   "ld",   Arguments::MEMORY, Arguments::REGISTER);
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
		MXT_CMD(jmp_c,   "jmp",  Arguments::CONSTANT);
		MXT_CMD(jr_c,    "jr",   Arguments::CONSTANT);
		MXT_CMD(jz_xc,   "jz",   Arguments::REGISTER, Arguments::CONSTANT);
		MXT_CMD(jrz_xc,  "jrz",  Arguments::REGISTER, Arguments::CONSTANT);
		MXT_CMD(call_c,  "call", Arguments::CONSTANT);
		MXT_CMD(cllr_c,  "cllr", Arguments::CONSTANT);
		MXT_CMD(cllz_xc, "cllz", Arguments::REGISTER, Arguments::CONSTANT);
		MXT_CMD(clrz_xc, "clrz", Arguments::REGISTER, Arguments::CONSTANT);
		MXT_CMD(ret,     "ret");
		MXT_CMD(push_x,  "push", Arguments::REGISTER);
		MXT_CMD(pop_x,   "pop",  Arguments::REGISTER);
		MXT_CMD(in_cx,   "in",   Arguments::CONSTANT, Arguments::REGISTER);
		MXT_CMD(out_cx,  "out",  Arguments::CONSTANT, Arguments::REGISTER);
		MXT_CMD(ei,      "ei");
		MXT_CMD(di,      "di");

#undef MXT_CMD
	}
}

#endif

