#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void handler(int);

char v = 0;

int main(int argc, char *argv[])
{
	signal(SIGINT, handler);

	int i = 0;
	while(++i)
	{
		printf("\b\b\b\b\b\b\b\b%8d", i);

		if(v)
		{
			printf("\nNaughty.\n");
			v = 0;
		}

		if(i == 1500000)
		{
			printf("\nNow i let you go.\n");
			signal(SIGINT, SIG_DFL);
		}
	}

	return 0;
}

void handler(int s)
{
	v = 1;
	signal(SIGINT, handler);
}

