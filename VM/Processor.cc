#include "Processor.h"
#include "Commands.h"

namespace vm { namespace cpu {

void Processor::registerOpcodes(commands_t& commands)
{
#define MXT_OP(id) commands.push_back(std::shared_ptr<Instruction>(new OP_##id))
	MXT_OP(hlt);
	MXT_OP(ld_xc);
	MXT_OP(ld_xy);
#undef MXT_OP
}

}}

