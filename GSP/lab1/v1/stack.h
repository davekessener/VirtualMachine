#ifndef DAV_GSP_1_STACK_H
#define DAV_GSP_1_STACK_H

#include "common.h"

typedef struct
{
	MXT_STYPE *stack;
	uint i, c;
} stack_t;

void stack_init(stack_t *);
void stack_push(stack_t *, MXT_STYPE);
MXT_STYPE stack_pop(stack_t *);
MXT_STYPE stack_top(const stack_t *);
const MXT_STYPE *stack_cbegin(const stack_t *);
const MXT_STYPE *stack_cend(const stack_t *);
uint stack_size(const stack_t *);
void stack_delete(stack_t *);


#endif

