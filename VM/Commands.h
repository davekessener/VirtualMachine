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
bool operator()(CPU&) const;\
}

		MXT_CMD(hlt, "hlt");
		MXT_CMD(ld_xc, "ld", Arguments::REGISTER, Arguments::CONSTANT);
		MXT_CMD(ld_xy, "ld", Arguments::REGISTER, Arguments::REGISTER);

#undef MXT_CMD
	}
}

#endif

