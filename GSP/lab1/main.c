#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "common.h"
#include "stack.h"
#include "opmap.h"
#include "io.h"

void op_add(stack_t *s) { stack_push(s, stack_pop(s) + stack_pop(s)); }
void op_sub(stack_t *s) { stack_push(s, -stack_pop(s) + stack_pop(s)); }
void op_mul(stack_t *s) { stack_push(s, stack_pop(s) * stack_pop(s)); }
void op_div(stack_t *s) { MXT_STYPE d = stack_pop(s); stack_push(s, stack_pop(s) / d); }
void op_p(stack_t *);
void op_f(stack_t *);
void op_c(stack_t *);
void op_d(stack_t *);
void op_r(stack_t *);

void init_operators(opmap_t *);
void evaluate(opmap_t *, const char *, stack_t *);

// # ===========================================================================

int main(int argc, char *argv[])
{
	stack_t stack;
	opmap_t ops;

	stack_init(&stack);
	opmap_init(&ops);

	init_operators(&ops);

	while(1)
	{
		char *input = read_line(stdin);

		if(!input) break;

		evaluate(&ops, input, &stack);

		free(input);
	}

	return EXIT_SUCCESS;
}

// # ===========================================================================

void evaluate(opmap_t *ops, const char *s, stack_t *stack)
{
	while(*s)
	{
		if(isws(*s))
		{
			++s;
		}
		else if(isnum(*s))
		{
			stack_push(stack, read_number(&s));
		}
		else
		{
			op_fn cb = opmap_get(ops, *s++);

			assert(cb);

			cb(stack);
		}
	}
}

// # ---------------------------------------------------------------------------

void init_operators(opmap_t *m)
{
	opmap_set(m, '+', op_add);
	opmap_set(m, '-', op_sub);
	opmap_set(m, '*', op_mul);
	opmap_set(m, '/', op_div);
	opmap_set(m, 'p', op_p);
	opmap_set(m, 'f', op_f);
	opmap_set(m, 'c', op_c);
	opmap_set(m, 'd', op_d);
	opmap_set(m, 'r', op_r);
}

// # ---------------------------------------------------------------------------

void op_p(stack_t *s)
{
	if(stack_size(s))
	{
		printf("%.2lf\n", stack_top(s));
#ifdef MXT_SET_EMPTY
		stack_pop(s);
#endif
	}
	else
	{
		printf("-empty-\n");
	}
}

void op_f(stack_t *s)
{
	const MXT_STYPE *i1 = stack_cbegin(s);
	const MXT_STYPE *i2 = stack_cend(s);
	int f = 1;

	printf("[");

	for(; i1 != i2 ; ++i1)
	{
		if(!f) printf(", ");
		printf("%.2lf", *i1);
		f = 0;
	}

	printf("]\n");

#ifdef MXT_SET_EMPTY
	stack_delete(s);
	stack_init(s);
#endif
}

void op_c(stack_t *s)
{
	stack_delete(s);
	stack_init(s);
}

void op_d(stack_t *s)
{
	assert(stack_size(s));
	stack_push(s, stack_top(s));
}

void op_r(stack_t *s)
{
	assert(stack_size(s) >= 2);

	MXT_STYPE v = stack_pop(s);
	MXT_STYPE u = stack_pop(s);
	stack_push(s, v);
	stack_push(s, u);
}

