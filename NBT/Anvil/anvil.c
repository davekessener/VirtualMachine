#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <zlib.h>
#include <assert.h>

enum
{
	EXTRACT,
	INSERT
};

typedef unsigned uint;

int getMode(const char *);
uint getOffset(int, int);
void anvil_insert(FILE *, FILE *, uint);
void anvil_extract(FILE *, FILE *, uint);

int main(int argc, char *argv[])
{
	const char **arg = (const char **) (argv + 1);
	int mode = getMode(argv[0]);
	int ac = argc - 1;

	if(ac > 0)
	{
		if(strcmp(*arg, "-e") == 0)
		{
			mode = EXTRACT;
			++arg; --ac;
		}
		else if(strcmp(*arg, "-i") == 0)
		{
			mode = INSERT;
			++arg; --ac;
		}
	}

	if(ac != 4)
	{
		fprintf(stderr, "ERR: Usage: %s [-i | -e] region.mca cx cz data.nbt\n", argv[0]);
		return EXIT_FAILURE;
	}

	const char *region_file = *arg++;

	int chunk_x = atoi(*arg++);
	int chunk_z = atoi(*arg++);

	const char *data_file = *arg;

	int cx = chunk_x % 32, cz = chunk_z % 32;
	if(cx < 0) cx += 32;
	if(cz < 0) cz += 32;

	FILE *rf = fopen(region_file, mode == INSERT ? "r+b" : "rb");
	FILE *df = fopen(data_file,   mode == INSERT ? "rb"  : "wb");

	if(!rf)
	{
		fprintf(stderr, "Couldn't open region file '%s'.\n", region_file);
		return EXIT_FAILURE;
	}

	if(!df)
	{
		fprintf(stderr, "Couldn't open data file '%s' for %s.\n", data_file, (mode == INSERT ? "read" : "write"));
		return EXIT_FAILURE;
	}

	if(mode == INSERT)
	{
		anvil_insert(rf, df, getOffset(x, z));
	}
	else
	{
		anvil_extract(rf, df, getOffset(x, z));
	}

	return EXIT_SUCCESS;
}

// # ===========================================================================

int getMode(const char *s)
{
	const char *t = s;
	while(*t)
	{
		if(*t == '/') s = t + 1;
		++t;
	}
	
	return strcmp(s, "anvil-insert") == 0 ? INSERT : EXTRACT;
}

uint getOffset(int x, int z)
{
	assert(x>=0&&x<32&&z>=0&&z<32);
	return (x | (z << 5)) << 2;
}

