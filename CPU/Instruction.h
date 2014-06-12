#ifndef __INSTRUCTION_H
#define __INSTRUCTION_H

#include <iostream>
#include <stdio.h>
#include <string>
using namespace std;

class Instruction
{
	public:
		bool matches(const char* &, WORD* &) const;
		int size() const;
		int ID() const;
		void print(const WORD*) const;
	private:
		Instruction(WORD, const char*, int, int);
		~Instruction();
		void skipWhitespace(const char* &);
		WORD readRegister(const char* &);
		
		WORD id;
		char *name;
		int arg1, arg2;
		
		friend class InstructionManager;
		static const int ARG_NONE, ARG_REGISTER, ARG_MEMORY, ARG_CONSTANT;
};

#endif

