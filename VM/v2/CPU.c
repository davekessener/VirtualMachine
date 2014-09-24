#include <assert.h>
#include "CPU.h"

#define MXT_MAX_PERI 16
#define MXT_MAX_INS 32
#define MXT_MEM_LIM ((WORD)(-1))
#define MXT_REGC (sizeof(WORD)<<3)

typedef void (*Ins_fn)(void);

Ins_fn instr[MXT_MAX_INS];

PSet_fn pset[MXT_MAX_PERI];
PGet_fn pget[MXT_MAX_PERI];
int run, pause, ie;
volatile int intr;

WORD ram[((int)MXT_MEM_LIM) + 1];
WORD *cpu_ram = ram;

WORD regs[MXT_REGC];
WORD * const pc = regs, * const sp = regs + MXT_REGC - 1;

void hlt();
void ld_xc();
void ld_xy();
void ld_xm();
void ld_mc();
void ld_mx();
void add_xy();
void sub_xy();
void and_xy();
void or_xy();
void not_x();
void xor_xy();
void xnor_xy();
void inc_x();
void dec_x();
void jmp_c();
void jmp_x();
void jp_c();
void jp_x();
void jz_xc();
void jz_xy();
void call_c();
void call_x();
void ret();
void push_x();
void pop_x();
void ei();
void di();
void in_cx();
void in_xy();
void out_cx();
void out_xy();

void push(WORD v)
{
	ram[(*sp)--] = v;
}

WORD pop(void)
{
	return ram[++*sp];
}

void cpu_check_perif( )
{
	static int perif = 0;

	if(!perif)
	{
		perif = 1;

		int i;
		for(i = 0 ; i < MXT_MAX_PERI ; ++i)
		{
			pset[i] = 0;
			pget[i] = 0;
		}
	}
}

void cpu_register(WORD r, PSet_fn set, PGet_fn get)
{
	cpu_check_perif();

	assert(r<MXT_MAX_PERI&&!pset[r]);
	
	pset[r] = set;
	pget[r] = get;
}

void cpu_run(void)
{
	WORD ins;

	run = 1; 
	pause = 0;
	intr = 0;
	ie = 0;
	
	cpu_check_perif();

	{
		int i;
		for(i = 0 ; i < MXT_REGC ; ++i) regs[i] = 0;
	}

	*pc = 0;
	*sp = MXT_MEM_LIM;

	{
		Ins_fn *pIns = instr;
		*pIns++ = &hlt;
		*pIns++ = &ld_xc;
		*pIns++ = &ld_xy;
		*pIns++ = &ld_xm;
		*pIns++ = &ld_mc;
		*pIns++ = &ld_mx;
		*pIns++ = &add_xy;
		*pIns++ = &sub_xy;
		*pIns++ = &and_xy;
		*pIns++ = &or_xy;
		*pIns++ = &not_x;
		*pIns++ = &xor_xy;
		*pIns++ = &xnor_xy;
		*pIns++ = &inc_x;
		*pIns++ = &dec_x;
		*pIns++ = &jmp_c;
		*pIns++ = &jmp_x;
		*pIns++ = &jp_c;
		*pIns++ = &jp_x;
		*pIns++ = &jz_xc;
		*pIns++ = &jz_xy;
		*pIns++ = &call_c;
		*pIns++ = &call_x;
		*pIns++ = &ret;
		*pIns++ = &push_x;
		*pIns++ = &pop_x;
		*pIns++ = &ei;
		*pIns++ = &di;
		*pIns++ = &in_cx;
		*pIns++ = &in_xy;
		*pIns++ = &out_cx;
		*pIns++ = &out_xy;
		assert(pIns-instr<=MXT_MAX_INS);
	}

	while(run)
	{
		if(intr && ie)
		{
			push(*pc);
			ie = 0;
			*pc = ram[intr];
			intr = 0;
		}

//		printf("@0x%04x: 0x%04x\n", *pc, ram[*pc]);

		ins = ram[(*pc)++];
		assert(ins<MXT_MAX_INS);
		instr[ins]();
	}
}

void cpu_interrupt(WORD i)
{
	if(ie) intr = i + 1;
}

int cpu_isrunning(void)
{
	return run;
}

void cpu_print(WORD lo, WORD hi)
{
	cpu_fprint(lo, hi, stdout);
}

void cpu_fprint(WORD lo, WORD hi, FILE *f)
{
	int i, j;

	for(i = 0 ; i < MXT_REGC ; ++i)
	{
		fprintf(f, "REG[%02x] = 0x%04x (%u)\n", i, regs[i], regs[i]);
	}

	for(i = lo / 0x10 ; i < (hi + 0x0f) / 0x10 ; ++i)
	{
		fprintf(f, "@0x%04x: ", i * 0x10);
		for(j = 0 ; j < 0x10 ; ++j)
		{
			fprintf(f, "%04x%c", ram[0x10 * i + j], j == 0x0f ? '\n' : ' ');
		}
	}
}

void hlt()
{
	run = 0;
}

void ld_xc()
{
	assert(ram[*pc]<MXT_REGC);
	regs[ram[*pc]] = ram[*pc+1];
	*pc += 2;
}

void ld_xy()
{
	assert(ram[*pc]<MXT_REGC);
	assert(ram[*pc+1]<MXT_REGC);
	regs[ram[*pc]] = regs[ram[*pc+1]];
	*pc += 2;
}

void ld_xm()
{
	assert(ram[*pc]<MXT_REGC);
	assert(ram[*pc+1]<MXT_REGC);
	regs[ram[*pc]] = ram[regs[ram[*pc+1]]];
	*pc += 2;
}

void ld_mc()
{
	assert(ram[*pc]<MXT_REGC);
	ram[regs[ram[*pc]]] = ram[*pc+1];
	*pc += 2;
}

void ld_mx()
{
	assert(ram[*pc]<MXT_REGC);
	assert(ram[*pc+1]<MXT_REGC);
	ram[regs[ram[*pc]]] = regs[ram[*pc+1]];
	*pc += 2;
}

void add_xy()
{
	assert(ram[*pc]<MXT_REGC);
	assert(ram[*pc+1]<MXT_REGC);
	regs[ram[*pc]] += regs[ram[*pc+1]];
	*pc += 2;
}

void sub_xy()
{
	assert(ram[*pc]<MXT_REGC);
	assert(ram[*pc+1]<MXT_REGC);
	regs[ram[*pc]] -= regs[ram[*pc+1]];
	*pc += 2;
}

void and_xy()
{
	assert(ram[*pc]<MXT_REGC);
	assert(ram[*pc+1]<MXT_REGC);
	regs[ram[*pc]] &= regs[ram[*pc+1]];
	*pc += 2;
}

void or_xy()
{
	assert(ram[*pc]<MXT_REGC);
	assert(ram[*pc+1]<MXT_REGC);
	regs[ram[*pc]] |= regs[ram[*pc+1]];
	*pc += 2;
}

void not_x()
{
	assert(ram[*pc]<MXT_REGC);
	regs[ram[*pc]] = ~regs[ram[*pc]];
	++*pc;
}

void xor_xy()
{
	assert(ram[*pc]<MXT_REGC);
	assert(ram[*pc+1]<MXT_REGC);
	regs[ram[*pc]] ^= regs[ram[*pc+1]];
	*pc += 2;
}

void xnor_xy()
{
	assert(ram[*pc]<MXT_REGC);
	assert(ram[*pc+1]<MXT_REGC);
	regs[ram[*pc]] = regs[ram[*pc]] ^ ~regs[ram[*pc+1]];
	*pc += 2;
}

void inc_x()
{
	assert(ram[*pc]<MXT_REGC);
	++regs[ram[(*pc)++]];
}

void dec_x()
{
	assert(ram[*pc]<MXT_REGC);
	--regs[ram[(*pc)++]];
}

void jmp_c()
{
	*pc += ram[*pc] + 1;
}

void jmp_x()
{
	assert(ram[*pc]<MXT_REGC);
	*pc += regs[ram[*pc]] + 1;
}

void jp_c()
{
	*pc = ram[*pc];
}

void jp_x()
{
	assert(ram[*pc]<MXT_REGC);
	*pc = regs[ram[*pc]];
}

void jz_xc()
{
	assert(ram[*pc]<MXT_REGC);
	if(regs[ram[*pc]])
	{
		*pc += 2;
	}
	else
	{
		*pc = ram[*pc+1];
	}
}

void jz_xy()
{
	assert(ram[*pc]<MXT_REGC);
	assert(ram[*pc+1]<MXT_REGC);
	if(regs[ram[*pc]])
	{
		*pc += 2;
	}
	else
	{
		*pc = regs[ram[*pc+1]];
	}
}

void call_c()
{
	push(*pc+1);
	*pc = ram[*pc];
}

void call_x()
{
	assert(ram[*pc]<MXT_REGC);
	push(*pc+1);
	*pc = regs[ram[*pc]];
}

void ret()
{
	*pc = pop();
}

void push_x()
{
	assert(ram[*pc]<MXT_REGC);
	push(regs[ram[(*pc)++]]);
}

void pop_x()
{
	assert(ram[*pc]<MXT_REGC);
	regs[ram[(*pc)++]] = pop();
}

void ei()
{
	ie = 1;
}

void di()
{
	ie = 0;
}

void in_cx()
{
	assert(pget[ram[*pc]]);
	assert(ram[*pc+1]<MXT_REGC);
	regs[ram[*pc+1]] = pget[ram[*pc]]();
	*pc += 2;
}

void in_xy()
{
	assert(ram[*pc]<MXT_REGC);
	assert(ram[*pc+1]<MXT_REGC);
	assert(pget[regs[ram[*pc]]]);
	regs[ram[*pc+1]] = pget[regs[ram[*pc]]]();
	*pc += 2;
}

void out_cx()
{
	assert(pset[regs[ram[*pc]]]);
	assert(ram[*pc+1]<MXT_REGC);
	pset[ram[*pc]](regs[ram[*pc+1]]);
	*pc += 2;
}

void out_xy()
{
	assert(pset[regs[ram[*pc]]]);
	assert(ram[*pc]<MXT_REGC);
	assert(ram[*pc+1]<MXT_REGC);
	pset[regs[ram[*pc]]](regs[ram[*pc+1]]);
	*pc += 2;
}

