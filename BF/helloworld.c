#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CHUNK 0x4000

void openFiles(int, char **, FILE **, FILE **);

int main(int argc, char *argv[])
{
	FILE *in, *out;
	unsigned char *ptr;

	openFiles(argc, argv, &in, &out);

	ptr = malloc(CHUNK);

	memset(ptr, 0, CHUNK);

	*ptr += 10;
	while(*ptr)
	{
		++ptr;
		*ptr += 7;
		++ptr;
		*ptr += 10;
		++ptr;
		*ptr += 3;
		++ptr;
		++*ptr;
		ptr -= 4;
		--*ptr;
	}
	++ptr;
	*ptr += 2;
	putc(*ptr, out);
	++ptr;
	++*ptr;
	putc(*ptr, out);
	*ptr += 7;
	putc(*ptr, out);
	putc(*ptr, out);
	*ptr += 3;
	putc(*ptr, out);
	++ptr;
	*ptr += 2;
	putc(*ptr, out);
	ptr -= 2;
	*ptr += 15;
	putc(*ptr, out);
	++ptr;
	putc(*ptr, out);
	*ptr += 3;
	putc(*ptr, out);
	*ptr -= 6;
	putc(*ptr, out);
	*ptr -= 8;
	putc(*ptr, out);
	++ptr;
	++*ptr;
	putc(*ptr, out);
	++ptr;
	putc(*ptr, out);

	return 0;
}

void openFiles(int argc, char *argv[], FILE **in, FILE **out)
{
	*in = stdin;
	*out = stdout;
}
