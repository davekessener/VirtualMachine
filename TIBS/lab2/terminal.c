#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/select.h>
#include "terminal.h"
#include "system.h"

struct termios old;
int changed = 0;

void interruptHandler(int);

void initializeTerminal(void)
{
	struct termios new;

	sys_tcgetattr(fileno(stdin), &old);
	new = old;
	new.c_lflag &= ~ICANON & ~ECHO;
	sys_tcsetattr(fileno(stdin), TCSANOW, &new);
	changed = 1;

	installSignalHandler(SIGINT, &interruptHandler);
}

char readTerminal(void)
{
	char c;
	fd_set set;

	FD_ZERO(&set);
	FD_SET(fileno(stdin), &set);

	if(select(fileno(stdin) + 1, &set, NULL, NULL, NULL) < 0)
	{
		reportError("select", errno);
	}

	if(read(fileno(stdin), &c, 1) < 0)
	{
		reportError("read", errno);
	}

	return c;
}

void restoreTerminal(void)
{
	if(changed)
	{
		sys_tcsetattr(fileno(stdin), TCSANOW, &old);
		changed = 0;
	}
}

void interruptHandler(int s)
{
	restoreTerminal();
	printf("\nGoodbye.\n");
	exit(0);
}

