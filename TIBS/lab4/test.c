#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>

int main(int argc, char *argv[])
{
	int f = open("/dev/tzm", O_RDONLY);

	if(f < 0)
	{
		fprintf(stderr, "ERR: Unexpected error %i\nAbort!\n", errno);
		return EXIT_FAILURE;
	}

	int t = open("/dev/tzm", O_RDONLY);
	int e = errno;

	close(f);

	if(t >= 0)
	{
		fprintf(stderr, "ERR: Unexpected success!\nAbort!\n");
		close(t);
		return EXIT_FAILURE;
	}

	if(e != EBUSY)
	{
		fprintf(stderr, "ERR: Unexpected failure code %i\nAbort!\n", e);
	}

	fprintf(stderr, "Failed successfully!\n");

	return 0;
}

