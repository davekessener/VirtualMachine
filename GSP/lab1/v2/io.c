#include <stdlib.h>
#include <assert.h>
#include "io.h"

int lookup(FILE *f)
{
	static int v = 0;

	if(f)
	{
		if(feof(f) || ((v = getc(f)) == EOF && ferror(f)))
		{
			fprintf(stderr, "Fatal read error. Abort!\n");
			exit(1);
		}
	}

	return v;
}

int read_int(uint *pv)
{
	int t = 0;
	uint v = 0;

	assert(pv);

	t = lookup(NULL);
	
	if(!isnum(t)) return ERR_IO_NONUM;

	if(t != '.')
	{
		for(; t >= '0' && t <= '9' ; t = lookup(stdin))
		{
			v = v * 10 + (t - '0');
		}
	}

	*pv = v;

	return ERR_NO;
}

int read_number(double *v)
{
	uint i = 0;
	double d = 0.0;
	int r = 0;

	assert(v);

	if((r = read_int(&i))) return r;

	if(lookup(NULL) == '.')
	{
		uint t = 0;

		lookup(stdin);

		if((r = read_int(&t))) return r;

		if(lookup(NULL) == '.') return ERR_IO_MALFORMED;

		for(d = t ; d >= 1.0 ; d *= 0.1);
	}

	*v = i + d;

	return ERR_NO;
}

