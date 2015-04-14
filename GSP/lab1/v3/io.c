#include "io.h"
#include "keypad.h"
#include "tft.h"

/* nutze macro um bequem zwischen normalen '\n' terminierten
   input und immediate return stdin zu wechseln */
#define lookup_touch lookup

#define MXT_RINGBUFSIZE 0x400

/* getc wrapper um gebuffertes stdin zu emulieren */
int my_getc(FILE *f)
{
    static int ringbuf[MXT_RINGBUFSIZE];
    static uint rbbegin = 0 , rbend = 0;

    int r = 0;
    
    rbbegin %= MXT_RINGBUFSIZE;
    rbend %= MXT_RINGBUFSIZE;
    
    if(rbbegin == rbend)
    {
        do
        {
            ringbuf[rbend++ % MXT_RINGBUFSIZE] = r = getc(f);   
        }
        while(!(r == '\n' || r == '\0' || r == '\t' || r == ' ' || r == EOF));
    }
    
    return ringbuf[rbbegin++];
}

int isnum(int c)
{ 
    return (int) (c >= '0' && c <= '9') || c == '.';
}

int isws(int c) 
{ 
    return c == '\n' || c == ' ' || c == '\t';
}

/* emuliert lookup() verhalten fuer touchpad input */
int lookup_touch(FILE *f)
{
    static int v = 0;
    
    if(f)
    {
        v = Get_Touch_Pad_Input();
        TFT_putc(v);
    }
    
    return v;
}

int lookup_stdin(FILE *f)
{
	static int v = 0;

	/* wenn ein FILE stream gegeben ist, liess neuen
	   lookup character ein. ansonster gebe gecached
	   character zurueck. */
	if(f)
	{
        v = my_getc(f);
	}

	return v;
}

/* lesen einen integer aus stdin */
int read_int(uint *pv)
{
	int t = 0;
	uint v = 0;

	t = lookup(NULL);
	
	if(!isnum(t)) return ERR_IO_NONUM;
    
	/* ein '.' wird als 0 interpretiert, um
	   zahlen wie '.25' zu erlauben */
	if(t != '.')
	{
		for(; t >= '0' && t <= '9' ; t = lookup(stdin))
		{
			/* da v unsigned ist kann man direkt gegen MAXSINT32 pruefen */
            if((v = v * 10 + (t - '0')) > MXT_MAXSINT32) return ERR_OVERFLOW;
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

//	assert(v);

	if((r = read_int(&i))) return r;

	/* wenn ein punkt am ende steht, muss ein integer
	   folgen. zahlen wie '3.' sind nicht erlaubt. */
	if(lookup(NULL) == '.')
	{
		uint t = 0;

		lookup(stdin);

		if((r = read_int(&t))) return r;

		/* eine zahl darf nicht mehr als einen punkt enthalten */
		if(lookup(NULL) == '.') return ERR_IO_MALFORMED;

		/* schiebe den decimalanteil in die nahckommerstellen */
		for(d = t ; d >= 1.0 ; d *= 0.1);
	}

	/* das ergebnis ist die summe aus dem integer- und decimalanteil. */
	*v = i + d;

	return ERR_NO;
}

