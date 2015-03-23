#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "common.h"
#include "stack.h"
#include "opmap.h"
#include "io.h"

/* funktionen: 
   bei der subtraction und der division
   muss darauf geachtet werden, dass
   die operanten umgedreht werden
*/
void op_add(stack_t *s) { stack_push(s, stack_pop(s) + stack_pop(s)); }
void op_sub(stack_t *s) { stack_push(s, -stack_pop(s) + stack_pop(s)); }
void op_mul(stack_t *s) { stack_push(s, stack_pop(s) * stack_pop(s)); }
void op_div(stack_t *s) { MXT_STYPE d = stack_pop(s); stack_push(s, stack_pop(s) / d); }
void op_p(stack_t *);
void op_f(stack_t *);
void op_c(stack_t *);
void op_d(stack_t *);
void op_r(stack_t *);

void init_operators(opmap_t *); /* initialisierung des lookup tables */
void evaluate(opmap_t *, const char *, stack_t *); /* evaluierung einer eingabe */

// # ===========================================================================

int main(int argc, char *argv[])
{
	stack_t stack; /* daten stack fuer die werte */
	opmap_t ops; /* lookup table der functionen */

	stack_init(&stack);
	opmap_init(&ops);

	init_operators(&ops);

	while(1)
	{
		char *input = read_line(stdin); /* liess eine zeile ein (siehe io.c) */

		if(!input) break; /* eine leere zeile signalisiert programmende */

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
			/* skip whitespace */
			++s;
		}
		else if(isnum(*s))
		{
			/* lese eine zahl ein (siehe io.c)
			   und pushe sie auf den stack (siehe stack.c)
			*/
			if(stack_push(stack, read_number(&s)))
			{
				error(ERR_STACKFULL);
			}
		}
		else
		{
			/* eine buchstabengebunde funktion ist gefragt (siehe opmap.c) */
			op_fn cb = opmap_get(ops, *s++);

			if(!cb)
			{
				error(ERR_UNKNOWN);
			}

			cb(stack);
		}
	}
}

// # ---------------------------------------------------------------------------

void init_operators(opmap_t *m)
{
	if(opmap_set(m, '+', op_add)) error(ERR_MAPFULL);
	if(opmap_set(m, '-', op_sub)) error(ERR_MAPFULL);
	if(opmap_set(m, '*', op_mul)) error(ERR_MAPFULL);
	if(opmap_set(m, '/', op_div)) error(ERR_MAPFULL);
	if(opmap_set(m, 'p', op_p)) error(ERR_MAPFULL);
	if(opmap_set(m, 'f', op_f)) error(ERR_MAPFULL);
	if(opmap_set(m, 'c', op_c)) error(ERR_MAPFULL);
	if(opmap_set(m, 'd', op_d)) error(ERR_MAPFULL);
	if(opmap_set(m, 'r', op_r)) error(ERR_MAPFULL);
}

// # ---------------------------------------------------------------------------

/* das MXT_SET_EMPTY macro entscheidet, ob der
   stack durch die inspectionsfunktionen 'p' und 'f'
   veraendert werden soll, oder nicht.
*/

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
	const MXT_STYPE *i1 = stack_cend(s);
	const MXT_STYPE *i2 = stack_cbegin(s);
	int f = 1;

	printf("[");

	do
	{
		--i1;
		if(!f) printf(", ");
		printf("%.2lf", *i1);
		f = 0;
	} while(i1 != i2);

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

