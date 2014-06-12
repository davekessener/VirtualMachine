#ifndef __EVALUATE_H
#define __EVALUATE_H

#include "include.h"
#include "tkn.h"
#include "dstack.h"
#include "logger.h"
#include <math.h>

#define SM_LIB
#include "stackmachine.h"
#undef SM_LIB

#ifdef EVAL_C

enum
{
	TOK_NONE, 
	TOK_PLUS,
	TOK_MINUS,
	TOK_AST,
	TOK_SLASH,
	TOK_EXP,
	TOK_LOG,
	TOK_LN,
	TOK_SIN,
	TOK_COS,
	TOK_TAN,
	TOK_OP,
	TOK_CP,
	TOK_PI,
	TOK_E,
	TOK_RUP,
	TOK_RDOWN,
	TOK_ROUND,
	TOK_LAND,
	TOK_LOR,
	TOK_LNOT,
	TOK_AND,
	TOK_OR,
	TOK_NOT,
	TOK_XOR,
	TOK_PERC,
	TOK_SHL,
	TOK_SHR
};

// Token structure - holds token-id
// and a number (if applicable)
typedef struct __token
{
	int type;
	FQP val;
} TOK;

// Parser structure - holds a tokenizer
// and a symbol table
typedef struct __parser
{
	TKN tokenizer;
	TABLE symtable;
} P;

void P_init(P*);
// Print a token to the output file
// via the specified output function
void P_print(P*, TOK);
void P_dispose(P*);

// Evaluate addition/subtraction
void evalAS(P *);
// Evaluate multiplication/division
void evalMD(P *);
// Evaluate exponentiation
void evalE(P *);
// Evaluate unary operations (negation, bitwise-negation, logical not ...)
void evalU(P *);
// Evaluate trig-fuctions, logarithm, etc.
void evalTL(P *);
// Evaluate bitwise operations
void evalAOX(P *);
// Evaluate logical operations
void evalLAO(P *);
// Evaluate a constant
void evalC(P *);

// Standard output function
// Just print to stdout
void stdEvalPrint(const char *);

// Function that returns a pointer to a
// static pointer to a fuction that returns
// void and takes a string as an argument.
// Dereferenced it functions as the
// output function used to print tokens.
typedef void (*logfPtr)(const char *);
logfPtr *pFn();
#endif

// Public function to set the output function
void setEvalOutput(void (*)(const char *));
// Read and tokenize a string, then print the
// intermediate representation of the
// infix mathematical expression in postfix notation.
int evaluate(const char *);

#endif

