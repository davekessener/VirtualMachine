#ifndef DAV_GSP_1_STACK_H
#define DAV_GSP_1_STACK_H

#include "common.h"

/* MXT_TYPE ist definiert in common.h,
   im regelfall entweder double oder int
*/
typedef struct
{
	MXT_STYPE *stack;
	/* 'i' ist actuelles buffer ende, 'c' ist buffer capacitaet. */
	uint i, c;
} stack_t;

void stack_init(stack_t *);
void stack_push(stack_t *, MXT_STYPE);
MXT_STYPE stack_pop(stack_t *);
MXT_STYPE stack_top(const stack_t *);
uint stack_size(const stack_t *);
void stack_delete(stack_t *);

/* iteratoren: */
const MXT_STYPE *stack_cbegin(const stack_t *);
const MXT_STYPE *stack_cend(const stack_t *);

#endif

