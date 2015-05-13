#ifndef DAV_CALC_H
#define DAV_CALC_H

#include <stdlib.h>
#include <malloc.h>
#include <string.h>

typedef unsigned uint;

typedef struct cell_t
{
	uint id;
	union
	{
		char binop;
		double num;
		uint id;
	} data;
	struct cell_t *left, *right;
} CELL;

enum
{
	C_NONE,
	C_NUM,
	C_VAR,
	C_ASSIGN,
	C_EXPRLIST,
	C_BINOP,
	C_NEG,
	C_CALL
};

void eval(CELL *);
CELL *assign(CELL *, CELL *);
CELL *fncall(uint, CELL *);
CELL *function(uint, CELL *);
CELL *variable(uint);
CELL *var_list(uint, CELL *);
CELL *expr_list(CELL *, CELL *);
CELL *append(CELL *, CELL *);
CELL *op_add(CELL *, CELL *);
CELL *op_sub(CELL *, CELL *);
CELL *op_mul(CELL *, CELL *);
CELL *op_div(CELL *, CELL *);
CELL *op_exp(CELL *, CELL *);
CELL *op_neg(CELL *);
CELL *number(double);

#endif

