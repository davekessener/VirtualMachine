#ifndef __STACKMACHINE_H
#define __STACKMACHINE_H

#include <math.h>
#include <string.h>
#include <stdio.h>
#include "include.h"
#include "dstack.h"
#include "tokenizer.h"

enum SM_ERR
{
	SM_ERR_NONE,
	SM_ERR_INPUT,
	SM_ERR_EMPTY,
	SM_ERR_UNKNOWN
};

enum SM_OP
{
	SM_OP_PUSH,
	SM_OP_ADD,	
	SM_OP_SUB,	
	SM_OP_MUL,	
	SM_OP_DIV,	
	SM_OP_EXP,	
	SM_OP_LOG,	
	SM_OP_LG,	
	SM_OP_LN,	
	SM_OP_SIN,	
	SM_OP_COS,	
	SM_OP_TAN,	
	SM_OP_NEG,	
	SM_OP_RUP,	
	SM_OP_RDWN,
	SM_OP_RND,	
	SM_OP_XSUM,
	SM_OP_LAND,
	SM_OP_LOR,
	SM_OP_LNOT,
	SM_OP_AND,
	SM_OP_OR,
	SM_OP_NOT,
	SM_OP_XOR,
	SM_OP_MOD,
	SM_OP_SHL,
	SM_OP_SHR
};

#ifdef SM_OPS
static const char *SM_OP_Instructions(int i)
{
	static const char *SM_OP_INS[] = {
		"push", 
		"add", 
		"sub", 
		"mul", 
		"div", 
		"exp", 
		"log", 
		"lg",
		"ln", 
		"sin", 
		"cos", 
		"tan", 
		"neg", 
		"rup", 
		"rdown", 
		"round",
		"xsum",
		"land",
		"lor",
		"lnot",
		"and",
		"or",
		"not",
		"xor",
		"mod",
		"shl",
		"shr"};
	
	return SM_OP_INS[i];
}
#endif

#define STR_EQ(s,v) strcmp(s,v)==0

#ifndef SM_LIB
#include "include.h"
#include "parameter.h"

#ifdef SM_PARAMS
#define FLAG_NONE 0
#define FLAG_VERBOSE 1

static const char **flagVerbose(void)
{
	static const char *fV[] = {
		"v",
		"verbose"};
	
	return fV;
}

typedef struct
{
	int flags;
} PARAMS;

void readParameter(PARAMS*, int, char **);
#endif
#endif

int  interpret(DS*, const char *);

#endif

