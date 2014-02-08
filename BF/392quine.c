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

	--*ptr;
	++ptr;
	*ptr += 2;
	++ptr;
	*ptr += 3;
	++ptr;
	++*ptr;
	++ptr;
	++*ptr;
	++ptr;
	*ptr += 3;
	ptr += 20;
	++*ptr;
	++ptr;
	++*ptr;
	++ptr;
	*ptr += 2;
	++ptr;
	*ptr += 3;
	++ptr;
	*ptr += 2;
	ptr += 2;
	*ptr += 3;
	++ptr;
	++*ptr;
	ptr += 33;
	++*ptr;
	++ptr;
	++*ptr;
	ptr += 2;
	*ptr += 3;
	ptr += 2;
	*ptr += 3;
	ptr += 5;
	*ptr += 3;
	++ptr;
	++*ptr;
	ptr += 9;
	*ptr += 2;
	++ptr;
	*ptr += 3;
	++ptr;
	*ptr += 3;
	++ptr;
	++*ptr;
	ptr += 2;
	*ptr += 3;
	ptr += 3;
	*ptr += 3;
	++ptr;
	++*ptr;
	++ptr;
	*ptr += 2;
	++ptr;
	*ptr += 3;
	ptr += 3;
	++*ptr;
	++ptr;
	++*ptr;
	++ptr;
	*ptr += 2;
	++ptr;
	*ptr += 3;
	++ptr;
	++*ptr;
	++ptr;
	++*ptr;
	ptr += 2;
	*ptr += 3;
	ptr += 7;
	++*ptr;
	++ptr;
	++*ptr;
	ptr += 3;
	++*ptr;
	++ptr;
	++*ptr;
	++ptr;
	*ptr += 2;
	++ptr;
	*ptr += 3;
	++ptr;
	*ptr += 3;
	++ptr;
	++*ptr;
	ptr += 2;
	*ptr += 3;
	ptr += 3;
	*ptr += 3;
	++ptr;
	++*ptr;
	++ptr;
	*ptr += 2;
	++ptr;
	*ptr += 3;
	++ptr;
	*ptr += 2;
	ptr += 2;
	++*ptr;
	++ptr;
	++*ptr;
	++ptr;
	*ptr += 2;
	++ptr;
	*ptr += 3;
	++ptr;
	++*ptr;
	++ptr;
	++*ptr;
	ptr += 2;
	*ptr += 3;
	ptr += 5;
	*ptr += 3;
	++ptr;
	++*ptr;
	ptr += 5;
	*ptr += 2;
	++ptr;
	*ptr += 3;
	++ptr;
	*ptr += 3;
	++ptr;
	++*ptr;
	ptr += 2;
	*ptr += 3;
	ptr += 3;
	*ptr += 3;
	++ptr;
	++*ptr;
	++ptr;
	*ptr += 3;
	++ptr;
	++*ptr;
	ptr += 2;
	*ptr += 3;
	ptr += 2;
	*ptr += 3;
	ptr += 2;
	*ptr += 2;
	while(*ptr)
	{
		while(*ptr)
		{
			ptr += 2;
			++*ptr;
			while(*ptr)
			{
				++ptr;
			}
			*ptr += 2;
			++ptr;
			*ptr += 2;
			while(*ptr)
			{
				--ptr;
			}
			--ptr;
			--*ptr;
		}
		++ptr;
		++*ptr;
		while(*ptr)
		{
			++ptr;
		}
		--ptr;
		++*ptr;
		--ptr;
		*ptr += 3;
		while(*ptr)
		{
			--ptr;
		}
		--ptr;
		++*ptr;
	}
	++ptr;
	++*ptr;
	while(*ptr)
	{
		++ptr;
	}
	*ptr += 4;
	++ptr;
	*ptr += 2;
	while(*ptr)
	{
		while(*ptr)
		{
			--ptr;
			*ptr += 16;
			++ptr;
			--*ptr;
		}
		--ptr;
		*ptr += 9;
		putc(*ptr, out);
		--ptr;
	}
	while(*ptr)
	{
		--*ptr;
		putc(*ptr, out);
		putc(*ptr, out);
		putc(*ptr, out);
	}

	return 0;
}

void openFiles(int argc, char *argv[], FILE **in, FILE **out)
{
	*in = stdin;
	*out = stdout;
}
