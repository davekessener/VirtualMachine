#include "CPU.h"

const WORD CPU::F_SIGNED = 1;
const WORD CPU::F_OVERFLOW = 2;
const WORD CPU::F_INT = 4;
const WORD CPU::F_EI = 8;
const WORD CPU::INT_TABLE = 0x0010;

CPU::CPU(void) : PC(R[15]), SP(R[14]), F(R[13]), A(R[12]), HL(R[11]), BC(R[10])
{
	ram = new WORD[RAM_SIZE];
	
	reset();
}

CPU::~CPU(void)
{
	delete[] ram;
}

void CPU::execute(void)
{
	if(isHalted()) return;
	counter++;
	
	if((counter & 0xf) == 0) interrupt(0);
	
	WORD instruction = peek_pc();
	
	switch(instruction)
	{
		case 0x0000: ld_xc();	break;
		case 0x0001: ld_xy();	break;
		case 0x0002: ld_xi();	break;
		case 0x0003: ld_ix();	break;
		case 0x0004: cmp_xy();	break;
		case 0x0005: jmp_c();	break;
		case 0x0006: jmp_cc();	break;
		case 0x0007: jz_xc();	break;
		case 0x0008: call_c();	break;
		case 0x0009: call_cc();	break;
		case 0x000A: ret();		break;
		case 0x000B: hlt();		break;
		case 0x000C: inc_x();	break;
		case 0x000D: dec_x();	break;
		case 0x000E: add_xy();	break;
		case 0x000F: sub_xy();	break;
		case 0x0010: mul_xy();	break;
		case 0x0011: div_xy();	break;
		case 0x0012: btst_xc();	break;
		case 0x0013: bset_xc();	break;
		case 0x0014: brst_xc();	break;
		case 0x0015: push_x();	break;
		case 0x0016: pop_x();	break;
		case 0x0017: ei();		break;
		case 0x0018: di();		break;
		default:
			printf("ERR: @0x%04hX Unrecognized instruction (0x%04hX)\n", PC - 1, instruction);
			hlt();
			break;
	}
	
	if(checkF(F_INT)) resetF(F_INT);
}

void CPU::writeMemory(const WORD *buf, ADDR a, int s)
{
	if(a + s > RAM_SIZE) s = RAM_SIZE - a;
	if(s < 0) s = 0;

	while(s-- > 0)
	{
		ram[a + s] = buf[s];
	}
}

void CPU::readMemory(WORD *buf, ADDR a, int s) const
{
	if(a + s > RAM_SIZE) s = RAM_SIZE - a;
	if(s < 0) s = 0;

	while(s-- > 0)
	{
		buf[s] = ram[a + s];
	}
}

void CPU::reset()
{
	for(int i = 0 ; i < 16 ; i++)
	{
		R[i] = 0x0000;
	}
	
	SP = 0xffff;
	isHlt = false;
	counter = 0l;
}

void CPU::printRegisters() const
{
	printf("# =========================================================\n");
	printf("# ----  Registers  ----------------------------------------\n");
	printf("# =========================================================\n");
	printf("     BC: 0x%04hX    A: 0x%04hX   SP: 0x%04hX\n", BC, A, SP);
	printf("     HL: 0x%04hX    F: 0x%04hX   PC: 0x%04hX\n", HL, F, PC);
	printf("# ---------------------------------------------------------\n");
	
	for(int i = 0 ; i < 4 ; i++)
	{
		printf("    ");
		for(int j = 0 ; j < 4 ; j++)
		{
			if(i * 4 + j < 10)
			{
				printf(" R%i: 0x%04hX  ", i * 4 + j, R[i * 4 + j]);
			}
			else
			{
				printf("R%i: 0x%04hX  ", i * 4 + j, R[i * 4 + j]);
			}
		}
		printf("\n");
	}
	
	printf("# ---------------------------------------------------------\n\n\n");
}

void CPU::printNextInstruction() const
{
	printf("### Next instruction @0x%04hX: 0x%04hX\n", PC, ram[PC]);
}

bool CPU::isHalted()
{
	return isHlt;
}

// # =============================================================================================

void CPU::ld_xc()
{
	WORD r1 = peek_pc() & 0xf;
	WORD v = peek_pc();
	
	R[r1] = v;
}

void CPU::ld_xy()
{
	WORD r1 = peek_pc() & 0xf;
	WORD r2 = peek_pc() & 0xf;
	
	R[r1] = R[r2];
}

void CPU::ld_xi()
{
	WORD r1 = peek_pc() & 0xf;
	WORD r2 = peek_pc() & 0xf;
	
	R[r1] = ram[R[r2]];
}

void CPU::ld_ix()
{
	WORD r1 = peek_pc() & 0xf;
	WORD r2 = peek_pc() & 0xf;
	
	ram[R[r1]] = R[r2];
}

void CPU::cmp_xy()
{
	WORD r1 = peek_pc() & 0xf;
	WORD r2 = peek_pc() & 0xf;
	
	A = 0;
	if(R[r1] < R[r2]) A |= 1;
	if(R[r1] <= R[r2]) A |= 2;
	if(R[r1] == R[r2]) A |= 4;
	if(R[r1] >= R[r2]) A |= 8;
	if(R[r1] > R[r2]) A |= 16;
}

void CPU::jmp_c()
{
	PC = peek_pc();
}

void CPU::jmp_cc()
{
	WORD c = peek_pc();
	WORD a = peek_pc();
	
	if(A & c)
	{
		PC = a;
	}
}

void CPU::jz_xc()
{
	WORD r = peek_pc();
	WORD c = peek_pc();
	
	if(R[r] == 0)
	{
		PC = c;
	}
}

void CPU::call_c()
{
	PUSH(PC);
	PC = peek_pc();
}

void CPU::call_cc()
{
	WORD c = peek_pc();
	WORD a = peek_pc();
	
	if(A & c)
	{
		PUSH(PC);
		PC = a;
	}
}

void CPU::ret()
{
	POP(PC);
}

void CPU::hlt()
{
	isHlt = true;
}

void CPU::inc_x()
{
	R[peek_pc()]++;
}

void CPU::dec_x()
{
	R[peek_pc()]--;
}

void CPU::add_xy()
{
	int v1, v2;
	WORD r1 = peek_pc() & 0xf, r2 = peek_pc() & 0xf;
	
	if(checkF(F_SIGNED))
	{
		v1 = (signed short) R[r1];
		v2 = (signed short) R[r2];
	}
	else
	{
		v1 = R[r1];
		v2 = R[r2];
	}
	
	v1 += v2;
	R[r1] = (WORD) (v1 & 0xffff);
	
	setF(F_OVERFLOW, checkF(F_SIGNED) ? (v1 < -0x8000 || v1 > 0x7fff) : v1 > 0xffff);
}

void CPU::sub_xy()
{
	int v1, v2;
	WORD r1 = peek_pc() & 0xf, r2 = peek_pc() & 0xf;
	
	if(checkF(F_SIGNED))
	{
		v1 = (signed short) R[r1];
		v2 = (signed short) R[r2];
	}
	else
	{
		v1 = R[r1];
		v2 = R[r2];
	}
	
	v1 -= v2;
	R[r1] = (WORD) (v1 & 0xffff);
	
	setF(F_OVERFLOW, checkF(F_SIGNED) ? (v1 < -0x8000 || v1 > 0x7fff) : v1 > 0xffff);
}

void CPU::mul_xy()
{
	int v1, v2;
	WORD r1 = peek_pc() & 0xf, r2 = peek_pc() & 0xf;
	
	if(checkF(F_SIGNED))
	{
		v1 = (signed short) R[r1];
		v2 = (signed short) R[r2];
	}
	else
	{
		v1 = R[r1];
		v2 = R[r2];
	}
	
	v1 *= v2;
	R[r1] = (WORD) (v1 & 0xffff);
	A = (WORD) ((v1 >> 16) & 0xffff);
	
	resetF(F_OVERFLOW);
}

void CPU::div_xy()
{
	int v1, v2;
	WORD r1 = peek_pc() & 0xf, r2 = peek_pc() & 0xf;
	
	if(checkF(F_SIGNED))
	{
		v1 = (signed short) R[r1];
		v2 = (signed short) R[r2];
	}
	else
	{
		v1 = R[r1];
		v2 = R[r2];
	}
	
	R[r1] = (WORD) ((v1 / v2) & 0xffff);
	A = (WORD) ((v1 % v2) & 0xffff);
	
	resetF(F_OVERFLOW);
}

void CPU::btst_xc()
{
	WORD r = peek_pc();
	WORD c = peek_pc();
	
	A = R[r] & c ? 1 : 0;
	
	resetF(F_OVERFLOW);
}

void CPU::bset_xc()
{
	WORD r = peek_pc();
	WORD c = peek_pc();
	
	R[r] |= c;
	
	resetF(F_OVERFLOW);
}

void CPU::brst_xc()
{
	WORD r = peek_pc();
	WORD c = peek_pc();
	
	R[r] &= ~c;
	
	resetF(F_OVERFLOW);
}

void CPU::push_x()
{
	PUSH(R[peek_pc()]);
}

void CPU::pop_x()
{
	POP(R[peek_pc()]);
}

void CPU::ei()
{
	setF(F_EI);
}

void CPU::di()
{
	resetF(F_EI);
}

// # =============================================================================================

void CPU::interrupt(WORD i)
{
	if(checkF(F_EI) && !checkF(F_INT))
	{
		setF(F_INT);
		PUSH(PC);
		PC = ram[INT_TABLE + i];
	}
}

WORD CPU::peek_pc()
{
	return ram[PC++];
}

WORD CPU::peek(ADDR a)
{
	return ram[a];
}

void CPU::poke(ADDR a, WORD v)
{
	ram[a] = v;
}

void CPU::setF(WORD f, bool set)
{
	if(set)
		F |= f;
	else
		F &= ~f;
}

void CPU::resetF(WORD f)
{
	F &= ~f;
}

bool CPU::checkF(WORD f)
{
	return (F & f) != 0;
}

