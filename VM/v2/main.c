#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "CPU.h"
#include "perif.h"

int main(int argc, char *argv[])
{
	if(argc != 2)
	{
		fprintf(stderr, "usage: %s kerlnel.bin\n", argv[0]);
		return 0;
	}

	{
		FILE *kernel;
		long l;

		if(!(kernel = fopen(argv[1], "rb")))
		{
			fprintf(stderr, "couldn't open kernel image '%s'! Abort!\n", argv[1]);
			return 1;
		}

		fseek(kernel, 0, SEEK_END);
		l = ftell(kernel);
		fseek(kernel, 0, SEEK_SET);
		fread(cpu_ram, l, 1, kernel);

		fclose(kernel);
	}

	cpu_register(0, screen_in, screen_out);
	cpu_register(1, keyboard_in, keyboard_out);

	if(fork())
	{
		cpu_run();
	}
	else
	{
		while(cpu_isrunning())
		{
			screen_update();
		}
	}

	return 0;
}

