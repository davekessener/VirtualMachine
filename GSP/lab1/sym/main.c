#include <stdio.h>
#include <stdlib.h>

enum
{
	NUMBER,
	SYMBOL,
	LIST
};

typedef struct cell_t
{
	union
	{
		double num;
		struct
		{
			char v, d;
		} sym;
		struct cell_t *car;
	} value;
	int type;
	struct cell_t *cdr;
} CELL;

CELL *derive(CELL *, d);

int main(int argc, char *argv[])
{
	return EXIT_SUCCESS;
}

CELL *derive(CELL *e, char d)
{
	assert(e);

	if(atomp(e))
	{
		return make_num(e->type == SYMBOL ? (e->value.sym.v == d ? 1 : (e->value.sym.d == d ? make_deriv(e->value.sym)) : 0);
	}
}

