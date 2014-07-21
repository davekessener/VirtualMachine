#include "size.h"

void getScreenSize(int *width, int *height)
{
	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

	*width = w.ws_col;
	*height = w.ws_row;
}

