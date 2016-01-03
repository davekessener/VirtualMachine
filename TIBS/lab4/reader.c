#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdint.h>

int main(int argc, char *argv[])
{
	int f = open("/dev/tzm", O_RDONLY);
	int64_t s;

	read(f, &s, sizeof(s));

	printf("%li (%lis)\n", s, s / 243);

	return 0;
}

