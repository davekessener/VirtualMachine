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
// # ===========================================================================
	MXT_OP(hlt);
// # ---------------------------------------------------------------------------
	MXT_OP(ld_xc);
	MXT_OP(ld_xy);
	MXT_OP(ld_xm);
	MXT_OP(ld_mc);
	MXT_OP(ld_mx);
// # ---------------------------------------------------------------------------
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
// # ---------------------------------------------------------------------------
	MXT_OP(add_xc);
	MXT_OP(sub_xc);
	MXT_OP(mul_xc);
	MXT_OP(div_xc);
	MXT_OP(mod_xc);
	MXT_OP(and_xc);
	MXT_OP(or_xc);
	MXT_OP(xor_xc);
// # ---------------------------------------------------------------------------
	MXT_OP(add_mc);
	MXT_OP(sub_mc);
	MXT_OP(mul_mc);
	MXT_OP(div_mc);
	MXT_OP(mod_mc);
	MXT_OP(and_mc);
	MXT_OP(or_mc);
	MXT_OP(xor_mc);
// # ---------------------------------------------------------------------------
	MXT_OP(add_mx);
	MXT_OP(sub_mx);
	MXT_OP(mul_mx);
	MXT_OP(div_mx);
	MXT_OP(mod_mx);
	MXT_OP(and_mx);
	MXT_OP(or_mx);
	MXT_OP(xor_mx);
// # ---------------------------------------------------------------------------
	MXT_OP(add_xm);
	MXT_OP(sub_xm);
	MXT_OP(mul_xm);
	MXT_OP(div_xm);
	MXT_OP(mod_xm);
	MXT_OP(and_xm);
	MXT_OP(or_xm);
	MXT_OP(xor_xm);
// # ---------------------------------------------------------------------------
	MXT_OP(inc_m);
	MXT_OP(dec_m);
	MXT_OP(not_m);
// # ---------------------------------------------------------------------------
	MXT_OP(jmp_c);
	MXT_OP(jmp_x);
	MXT_OP(jmp_m);
	MXT_OP(jz_xc);
	MXT_OP(jz_xy);
	MXT_OP(jz_xm);
	MXT_OP(jz_mc);
	MXT_OP(jz_mx);
	MXT_OP(jz_mm);
	MXT_OP(call_c);
	MXT_OP(call_x);
	MXT_OP(call_m);
	MXT_OP(cllz_xc);
	MXT_OP(cllz_xy);
	MXT_OP(cllz_xm);
	MXT_OP(cllz_mc);
	MXT_OP(cllz_mx);
	MXT_OP(cllz_mm);
	MXT_OP(ret);
// # ---------------------------------------------------------------------------
	MXT_OP(jr_c);
	MXT_OP(jr_x);
	MXT_OP(jr_m);
	MXT_OP(jrz_xc);
	MXT_OP(jrz_xy);
	MXT_OP(jrz_xm);
	MXT_OP(jrz_mc);
	MXT_OP(jrz_mx);
	MXT_OP(jrz_mm);
	MXT_OP(cllr_c);
	MXT_OP(cllr_x);
	MXT_OP(cllr_m);
	MXT_OP(clrz_xc);
	MXT_OP(clrz_xy);
	MXT_OP(clrz_xm);
	MXT_OP(clrz_mc);
	MXT_OP(clrz_mx);
	MXT_OP(clrz_mm);
// # ---------------------------------------------------------------------------
	MXT_OP(push_c);
	MXT_OP(push_x);
	MXT_OP(push_m);
	MXT_OP(pop_x);
	MXT_OP(pop_m);
// # ---------------------------------------------------------------------------
	MXT_OP(in_cx);
	MXT_OP(in_cm);
	MXT_OP(in_xy);
	MXT_OP(in_xm);
	MXT_OP(in_mx);
	MXT_OP(in_mm);
	MXT_OP(out_cc);
	MXT_OP(out_cx);
	MXT_OP(out_cm);
	MXT_OP(out_xc);
	MXT_OP(out_xy);
	MXT_OP(out_xm);
	MXT_OP(out_mc);
	MXT_OP(out_mx);
	MXT_OP(out_mm);
// # ---------------------------------------------------------------------------
	MXT_OP(ei);
	MXT_OP(di);
// # ===========================================================================
#undef MXT_OP
}

void Processor::attachPeripherials(void)
{
	attach(new BasicVGA);
	attach(new BasicKeyboard);
}

}}

