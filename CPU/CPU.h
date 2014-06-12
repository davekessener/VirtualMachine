#ifndef __CPU_H
#define __CPU_H

#include <iostream>
#include <stdio.h>
#include "ddef.h"
using namespace std;

#define RAM_SIZE 0x10000
#define PUSH(x) (ram[--SP] = (x))
#define POP(x) ((x) = ram[SP++])

class CPU
{
	public:
		CPU();
		~CPU();
		void execute();
		void writeMemory(const WORD*, ADDR, int);
		void readMemory(WORD*, ADDR, int) const;
		void printRegisters() const;
		void printNextInstruction() const;
		void reset();
		bool isHalted();
	private:
		WORD *ram;
		WORD R[16];
		long int counter;
		int IM;
		bool isHlt;
		WORD &PC, &SP, &F, &A, &HL, &BC;
		
		void ld_xc();
		void ld_xy();
		void ld_xi();
		void ld_ix();
		void cmp_xy();
		void jmp_c();
		void jmp_cc();
		void jz_xc();
		void call_c();
		void call_cc();
		void ret();
		void hlt();
		void inc_x();
		void dec_x();
		void add_xy();
		void sub_xy();
		void mul_xy();
		void div_xy();
		void btst_xc();
		void bset_xc();
		void brst_xc();
		void push_x();
		void pop_x();
		void ei();
		void di();
		
		void interrupt(WORD);
		WORD peek_pc();
		WORD peek(ADDR);
		void poke(ADDR, WORD);
		void setF(WORD, bool = true);
		void resetF(WORD);
		bool checkF(WORD);
		
		static const WORD F_SIGNED, F_OVERFLOW, F_INT, F_EI;
		static const WORD INT_TABLE;
};

#endif

