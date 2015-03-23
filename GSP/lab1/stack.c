#include "stack.h"
#include <malloc.h>
#include <assert.h>

void stack_init(stack_t *s)
{
	s->stack = NULL;
	s->i = s->c = 0;
}

void stack_push(stack_t *s, MXT_STYPE v)
{
	/* wenn der stack voll ist, dann wird die capazitaet
	   um das doppelte + 1 erhoeht.
	*/
	if(s->i == s->c)
	{
		s->stack = realloc(s->stack, (s->c = 2 * s->c + 1) * sizeof(MXT_STYPE));
	}

	s->stack[s->i++] = v;
}

MXT_STYPE stack_pop(stack_t *s)
{
	assert(s->i);

	MXT_STYPE v = s->stack[--s->i]; /* speicher obersten wert zwischen */

	/* wenn der stack nach pop leer ist, loesche buffer */
	if(!s->i)
	{
		free(s->stack);
		s->stack = NULL;
		s->i = s->c = 0;
	}
	/* wenn der stack unter 25% buffercapazitaet rutsch, verkleiner buffer */
	else if(s->i < s->c / 4)
	{
		s->stack = realloc(s->stack, (s->c >>= 1) * sizeof(MXT_STYPE));
	}

	return v;
}

MXT_STYPE stack_top(const stack_t *s)
{
	assert(s->i);
	return s->stack[s->i - 1];
}

const MXT_STYPE *stack_cbegin(const stack_t *s)
{
	return s->stack;
}

const MXT_STYPE *stack_cend(const stack_t *s)
{
	return s->stack + s->i;
}

uint stack_size(const stack_t *s)
{
	return s->i;
}

void stack_delete(stack_t *s)
{
	free(s->stack);
}

