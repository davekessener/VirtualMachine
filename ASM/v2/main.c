#include <stdio.h>
#include <stdlib.h>
#include "assembler.h"

char readnext(void);
void writenext(char);

FILE *i_f, *o_f;

int main(int argc, char *argv[])
{
	i_f = argc >= 1 ? fopen(argv[1], "r") : stdin;
	o_f = argc >= 2 ? fopen(argv[2], "w") : stdout;

	assemble(&readnext, &writenext);

	return 0;
}

char readnext(void)
{
	return getc(i_f);
}

void writenext(char c)
{
	putc(c, i_f);
}

