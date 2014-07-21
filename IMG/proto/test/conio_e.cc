#include "conio_e.h"

struct termios *orig_termios()
{
	static struct termios t;

	return &t;
}

void prepare_terminal()
{
	struct termios new_termios;

	tcgetattr(0, orig_termios());
	memcpy(&new_termios, orig_termios(), sizeof(new_termios));
//	atexit(restore_terminal);
	cfmakeraw(&new_termios);
	tcsetattr(0, TCSANOW, &new_termios);
}

void restore_terminal()
{
    tcsetattr(0, TCSANOW, orig_termios());
}

int kbhit()
{
	struct timeval tv = { 0L, 0L };
	fd_set fds;

	FD_ZERO(&fds);
	FD_SET(STDIN_FILENO, &fds);

	return select(sizeof(fds) * 8, &fds, NULL, NULL, &tv);
}

int getch()
{
	int r;
	unsigned char c;

	return ((r = read(0, &c, sizeof(c))) < 0) ? r : c;
}

