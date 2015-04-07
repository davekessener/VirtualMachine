#include <stdlib.h>
#include <assert.h>
#include "io.h"

int lookup(FILE *f)
{
	static int v = 0;

	/* wenn ein FILE stream gegeben ist, liess neuen
	   lookup character ein. ansonster gebe gecached
	   character zurueck. */
	if(f)
	{
		/* nutzer versucht ueber EOS hinaus zu lesen. Abort */
		if(feof(f) || ((v = getc(f)) == EOF && ferror(f)))
		{
			fprintf(stderr, "Fatal read error. Abort!\n");
			exit(1);
		}
	}

	return v;
}

/* lesen einen integer aus stdin */
int read_int(uint *pv)
{
	int t = 0;
	uint v = 0;

	assert(pv);

	t = lookup(NULL);
	
	if(!isnum(t)) return ERR_IO_NONUM;

	/* ein '.' wird als 0 interpretiert, um
	   zahlen wie '.25' zu erlauben */
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
	/* ein double besteht aus einem integer anteil,
	   potenziell gefolgt von einen '.' und einem
	   weiteren integeranteil */
	uint i = 0;
	double d = 0.0;
	int r = 0;

	assert(v);

	if((r = read_int(&i))) return r;

	/* wenn ein punkt am ende steht, muss ein integer
	   folgen. zahlen wie '3.' sind nicht erlaubt. */
	if(lookup(NULL) == '.')
	{
		uint t = 0;

		lookup(stdin);

		if((r = read_int(&t))) return r;

		if(lookup(NULL) == '.') return ERR_IO_MALFORMED;

		/* schiebe den decimalanteil in die nahckommerstellen */
		for(d = t ; d >= 1.0 ; d *= 0.1);
	}

	/* das ergebnis ist die summe aus dem integer- und decimalanteil. */
	*v = i + d;

	return ERR_NO;
}

