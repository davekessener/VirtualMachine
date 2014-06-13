#include "Processor.h"
#include "Commands.h"
#include "Periphery.h"
#include "VGA.h"
#include "Keyboard.h"

namespace vm { namespace cpu {

using namespace periphery;

void Processor::registerOpcodes(commands_t& commands)
{
#define MXT_OP(id) commands.push_back(std::shared_ptr<Instruction>(new OP_##id))
	MXT_OP(hlt);
	MXT_OP(ld_xc);
	MXT_OP(ld_xy);
	MXT_OP(ld_xm);
	MXT_OP(ld_mx);
	MXT_OP(inc_x);
	MXT_OP(dec_x);
	MXT_OP(add_xy);
	MXT_OP(sub_xy);
	MXT_OP(mul_xy);
	MXT_OP(div_xy);
	MXT_OP(mod_xy);
	MXT_OP(and_xy);
	MXT_OP(or_xy);
	MXT_OP(xor_xy);
	MXT_OP(not_x);
	MXT_OP(jmp_c);
	MXT_OP(jz_xc);
	MXT_OP(call_c);
	MXT_OP(cllz_xc);
	MXT_OP(ret);
	MXT_OP(push_x);
	MXT_OP(pop_x);
	MXT_OP(in_cx);
	MXT_OP(out_cx);
	MXT_OP(ei);
	MXT_OP(di);
#undef MXT_OP
}

void Processor::attachPeripherials(void)
{
	attach(new BasicVGA);
	attach(new BasicKeyboard);
}

}}

