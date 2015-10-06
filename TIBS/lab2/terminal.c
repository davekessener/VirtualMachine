#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/select.h>
#include "terminal.h"

struct termios old;

void initializeTerminal(void)
{
	struct termios new;

	tcgetattr(fileno(stdin), &old);
	new = old;
	new.c_lflag &= ~ICANON & ~ECHO;
	tcsetattr(fileno(stdin), TCSANOW, &new);
}

int readTerminal(char *c)
{
	int r;
	fd_set set;

	FD_ZERO(&set);
	FD_SET(fileno(stdin), &set);

	r = select(fileno(stdin) + 1, &set, NULL, NULL, NULL);

	if(r > 0)
	{
		read(fileno(stdin), c, 1);
	}

	return r;
}

void restoreTerminal(void)
{
	tcsetattr(fileno(stdin), TCSANOW, &old);
}

