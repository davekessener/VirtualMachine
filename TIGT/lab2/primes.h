#ifndef DAV_TIGT_2_PRIMES_H
#define DAV_TIGT_2_PRIMES_H

typedef unsigned char BYTE;
typedef unsigned int uint;
typedef uint bitfield_t;

uint sieb(bitfield_t *, uint);
void write_primes(bitfield_t *, uint *, uint);

#endif

