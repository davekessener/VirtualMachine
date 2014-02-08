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

	++ptr;
	*ptr = getc(in);
	while(*ptr)
	{
		--ptr;
		*ptr += 6;
		while(*ptr)
		{
			++ptr;
			*ptr -= 8;
			--ptr;
			--*ptr;
		}
		++ptr;
		while(*ptr)
		{
			++ptr;
			++*ptr;
			--ptr;
			--*ptr;
		}
		++ptr;
		while(*ptr)
		{
			--ptr;
			++*ptr;
			ptr += 2;
			++*ptr;
			--ptr;
			--*ptr;
		}
		++ptr;
		while(*ptr)
		{
			--ptr;
			++*ptr;
			++ptr;
			--*ptr;
		}
		ptr -= 2;
		while(*ptr)
		{
			--*ptr;
			while(*ptr)
			{
				--*ptr;
				while(*ptr)
				{
					--*ptr;
					while(*ptr)
					{
						--*ptr;
						while(*ptr)
						{
							--*ptr;
							while(*ptr)
							{
								--*ptr;
								while(*ptr)
								{
									--*ptr;
									while(*ptr)
									{
										--*ptr;
										while(*ptr)
										{
											--*ptr;
											while(*ptr)
											{
												while(*ptr)
												{
													--*ptr;
												}
												++ptr;
												*ptr -= 9;
												--ptr;
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
		*ptr = getc(in);
	}
	*ptr += 6;
	while(*ptr)
	{
		++ptr;
		*ptr += 8;
		--ptr;
		--*ptr;
	}
	*ptr += 10;
	++ptr;
	putc(*ptr, out);
	--ptr;
	putc(*ptr, out);

	return 0;
}

void openFiles(int argc, char *argv[], FILE **in, FILE **out)
{
	*in = stdin;
	*out = stdout;
}
