#include <stdlib.h>
#include <assert.h>
#include "stack.h"

MXT_STYPE stack[MXT_STACKCAP];
size_t i = 0;

int stack_push(MXT_STYPE v)
{
	if(i == MXT_STACKCAP) return ERR_STACK_FULL;
	stack[i++] = v;
	return ERR_NO;
}

int stack_pop(MXT_STYPE *p)
{
	if(!i) return ERR_STACK_EMPTY;
	--i;
	if(p) *p = stack[i];
	return ERR_NO;
}

int stack_empty(void)
{
	return !i;
}

