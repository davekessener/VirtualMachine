#include <stdio.h>
#include <stdlib.h>
#include "primes.h"

#define MXT_MAXPRIME 997
#define BC (sizeof(bitfield_t) << 3)

int main(int argc, char *argv[])
{
	bitfield_t prime_buf[MXT_MAXPRIME / BC + 1];
	uint primes[200];
	
	uint pc = sieb(prime_buf, MXT_MAXPRIME + 1);

	write_primes(prime_buf, primes, MXT_MAXPRIME + 1);

	printf("%u-bit\n%u primes:\n", sizeof(bitfield_t) << 3, pc);

	uint i;
	for(i = 0 ; i < pc ; ++i)
	{
		printf("% 3u ", primes[i]);
		if((i + 1) % 10 == 0) printf("\n");
	}

	printf("\n");

	return EXIT_SUCCESS;
}

