#include <stdio.h>
#include <stdlib.h>
#include "vmaccess.h"
#include "vmem.h"

int main(int argc, char *argv[])
{
	int i, l = VMEM_NFRAMES + 1;

	for(i = 0 ; i < l * VMEM_PAGESIZE ; ++i)
	{
		vmem_write(i, i);
	}

	for(i = 0 ; i < l * VMEM_PAGESIZE ; ++i)
	{
		printf("%08x%c", vmem_read(i), (i + 1) % VMEM_PAGESIZE ? ' ' : '\n');
	}

	return 0;
}

