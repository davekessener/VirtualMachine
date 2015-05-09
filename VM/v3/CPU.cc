#include "CPU.h"
#include <cassert>
#include <cstring>

namespace vm {

/* registers 0x00 - 0x0f : r0 - r15 : user registers
   registers 0x10 - 0x1f : s0 - s15 : system registers
   registers 0x20 - 0x2f : t0 - t15 : temp registers
*/

#define SP reg[0 + 0x10]
#define IX reg[1 + 0x10]
#define A  reg[2 + 0x10]
#define BL reg[3 + 0x10]
#define HL reg[4 + 0x10]

void hlt( );
void ld_xc( );
void ld_xx( );
void ldl_xm( );
void ldh_xm( );
void ldl_mx( );
void ldh_mx( );
void add_xc( );
void add_xx( );
void sub_xx( );
void and_xc( );
void and_xx( );
void or_xc( );
void or_xx( );
void xor_xc( );
void xor_xx( );
void inc_x( );
void dex_x( );
void jp_c( );
void jp_x( );
void jz_xc( );
void jz_xx( );
void acall_c( );
void acall_x( );
void push_x( );
void pop_x( );

using routine_fn = void (*)(void);

BYTE ram[0x10000];
WORD reg[0x30];
WORD PC;
uint active_int = 0;
bool running;

routine_fn inst[] =
{
	hlt, // 0x00
	ld_xc,
	ld_xx,
	ldl_xm,
	ldh_xm,
	ldl_mx,
	ldh_mx,
	add_xc,
	add_xx, // 0x08
	sub_xx,
	and_xx,
	or_xx,
	xor_xx,
	inc_x,
	dex_x,
	jp_c,
	jp_x, // 0x10
	jz_xc,
	jz_xx,
	acall_c,
	acall_x,
	push_x,
	pop_x
};

void set_ram(size_t i, const BYTE *d, size_t l)
{
	assert(i + l < 0x10000);
	std::memcpy(ram + i, d, l);
}

const BYTE *get_ram(void)
{
	return ram;
}

void reset(void)
{
	PC = 0;
	SP = 0xffff;
	active_int = 0;
}

BYTE next( ) { return ram[PC++]; }
WORD nextw( ) { WORD w = *((WORD *) (ram + PC)); PC += 2; return w; }

void push(WORD v)
{
	ram[--SP] = v;
}

WORD pop(void)
{
	return ram[SP++];
}

void interrupt(void)
{
}

void run(void)
{
	reset();

	running = true;

	while(running)
	{
		if(active_int) interrupt();
		else
		{
			BYTE i = next();
			std::cout << "Next is " << (DWORD)i << "\n";
			inst[i]();
		}
	}
}

void print_reg(std::ostream& os)
{
	os << "PC: " << PC << "\n";
	for(uint i = 0 ; i < 0x30 ; ++i)
	{
		os << "r" << i << " = " << reg[i] << "\n";
	}
}

// # ===========================================================================

void hlt(void)
{
	running = false;
}

void ld_xc(void)
{
	BYTE x = next();
	WORD c = nextw();

	reg[x] = c;
}

void ld_xx(void)
{
	BYTE x1 = next();
	BYTE x2 = next();

	reg[x1] = reg[x2];
}

void ldl_xm(void)
{
	BYTE x = next();
	BYTE m = next();
	SWORD off = nextw();

	reg[x] = (reg[x] & 0xff00 ) | ram[reg[m] + off];
}

void ldh_xm(void)
{
	BYTE x = next();
	BYTE m = next();
	SWORD off = nextw();

	reg[x] = (reg[x] & 0xff) | (ram[reg[m] + off] << 8);
}

void ldl_mx(void)
{
	BYTE m = next();
	SWORD off = nextw();
	BYTE x = next();

	ram[reg[m] + off] = reg[x] & 0xff;
}

void ldh_mx(void)
{
	BYTE m = next();
	SWORD off = nextw();
	BYTE x = next();

	ram[reg[m] + off] = (reg[x] >> 8) & 0xff;
}

void add_xc(void)
{
	BYTE x = next();
	WORD c = nextw();

	reg[x] += c;
}

void add_xx(void)
{
	BYTE x1 = next();
	BYTE x2 = next();

	reg[x1] += reg[x2];
}

void sub_xx(void)
{
	BYTE x1 = next();
	BYTE x2 = next();

	reg[x1] -= reg[x2];
}

void and_xc(void)
{
	BYTE x = next();
	WORD c = nextw();

	reg[x] &= c;
}

void and_xx(void)
{
	BYTE x1 = next();
	BYTE x2 = next();

	reg[x1] &= reg[x2];
}

void or_xc(void)
{
	BYTE x = next();
	WORD c = nextw();

	reg[x] |= c;
}

void or_xx(void)
{
	BYTE x1 = next();
	BYTE x2 = next();

	reg[x1] |= reg[x2];
}

void xor_xc(void)
{
	BYTE x = next();
	WORD c = nextw();

	reg[x] ^= c;
}

void xor_xx(void)
{
	BYTE x1 = next();
	BYTE x2 = next();

	reg[x1] ^= reg[x2];
}

void inc_x(void)
{
	BYTE x = next();

	++reg[x];
}

void dex_x(void)
{
	BYTE x = next();

	--reg[x];
}

void jp_c(void)
{
	WORD a = nextw();

	PC = a;
}

void jp_x(void)
{
	BYTE x = next();

	PC = reg[x];
}

void jz_xc(void)
{
	BYTE x = next();
	WORD a = nextw();

	if(!reg[x])
	{
		PC = a;
	}
}

void jz_xx(void)
{
	BYTE x1 = next();
	BYTE x2 = next();

	if(!reg[x1])
	{
		PC = reg[x2];
	}
}

void acall_c(void)
{
	WORD a = nextw();

	push(PC);

	PC = a;
}

void acall_x(void)
{
	BYTE x = next();

	push(PC);

	PC = reg[x];
}

void push_x(void)
{
	BYTE x = next();

	push(reg[x]);
}

void pop_x(void)
{
	BYTE x = next();

	reg[x] = pop();
}

}

