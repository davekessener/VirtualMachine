#include "Instruction.h"

const int Instruction::ARG_NONE = 0;
const int Instruction::ARG_REGISTER = 1;
const int Instruction::ARG_MEMORY = 2;
const int Instruction::ARG_CONSTANT = 3;

Instruction::Instruction(WORD w, const char *n, int a1, int a2) : id(w), arg1(a1), arg2(a2)
{
	name = strdup(n);
}

Instruction::~Instruction()
{
	free(name);
}

bool Instruction::matches(const char *ins&, WORD *out&) const
{
	const char *in = ins;
	for(int i = 0 ; name[i] != '\0' ; i++)
	{
		if(name[i] != *in++) return false;
	}
	
	skipWhitespace(in);
	
	WORD a1 = 0, a2 = 0;
	
	switch(arg1)
	{
	case ARG_NONE:
		if(*in == '\n')
		{
			ins = in;
			*out++ = id;
			return true;
		}
		else
		{
			return false;
		}
		break;
	case ARG_REGISTER:
		if(*in == 'r')
		{
			a1 = readRegister(++in);
		}
		else
		{
			return false;
		}
		break;
	case ARG_MEMORY:
		if(
		break;
	case ARG_CONSTANT:
		break;
}

WORD inline Instruction::readRegister(const char *in&)
{
	WORD r = 0;
	
	while(*in >= '0' && *in <= '9')
	{
		r *= 10;
		r += *in - '0';
		in++;
	}
	
	return r;
}

void inline Instruction::skipWhitespace(const char *str&)
{
	while(*str == ' ' || *str == '\t') str++;
}

int Instruction::size() const
{
	return arg1 > 0 ? (arg2 > 0 ? 3 : 2) : 1;
}

int Instruction::ID() const
{
	return id;
}

void Instruction::print(const WORD *buf) const
{
	printf("INS(0x%04hX): %s ", id, name);
	
	if(arg1 == ARG_REGISTER)
	{
		printf("r%i", buf[1]);
	}
	else if(arg1 == ARG_MEMORY)
	{
		printf("(r%i)", buf[1]);
	}
	else if(arg1 == ARG_CONSTANT)
	{
		printf("0x%04hX", buf[1]);
	}
	
	if(arg2 == ARG_REGISTER)
	{
		printf(",r%i", buf[2]);
	}
	else if(arg2 == ARG_MEMORY)
	{
		printf(",(r%i)", buf[2]);
	}
	else if(arg2 == ARG_CONSTANT)
	{
		printf(",0x%04hX", buf[2]);
	}
	
	printf("\n");
}

