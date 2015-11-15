#include "primes.h"

#define BC (sizeof(bitfield_t) << 3)
#define BM (BC-1)
#define BIT 1
#define MXT_FIRSTPRIME 2

uint get(bitfield_t *a, uint b)
{
	return (a[b / BC] >> (b & BM)) & 1;
}

void reset(bitfield_t *a, uint b)
{
	a[b / BC] &= ~(1 << (b & BM));
}

uint sieb(bitfield_t *a, uint c)
{
	uint i, j;

	for(i = 0 ; i < ((c + BM) / BC) ; ++i)
	{
		a[i] = -1;
	}

	for(i = MXT_FIRSTPRIME ; i * i < c ; ++i)
	{
		if(get(a, i))
		{
			for(j = i ; i * j < c ; ++j)
			{
				reset(a, i * j);
			}
		}
	}

	for(i = MXT_FIRSTPRIME, j = 0 ; i < c ; ++i)
	{
		if(get(a, i)) ++j;
	}

	return j;
}

void write_primes(bitfield_t *src, uint *dst, uint c)
{
	uint i;
	for(i = MXT_FIRSTPRIME ; i < c ; ++i)
	{
		if(get(src, i)) *dst++ = i;
	}
}

