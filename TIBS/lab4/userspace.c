#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "terminal.h"
#include "speed.h"

void speed_changed(int64_t);

int main(int argc, char *argv[])
{
	int last = '\n';
	struct timespec ts;

	initializeTerminal();
	tzm_initialize(&speed_changed);

	while(1)
	{
		int c = readTerminal();

		putc(c, stdout);
		fflush(stdout);

		if(c == '\n' && last == '\n')
			break;

		clock_gettime(CLOCK_REALTIME, &ts);
		tzm_pressed(c, ts.tv_sec * 1000000000L + ts.tv_nsec);

		last = c;
	}
	
	tzm_cleanup();
	restoreTerminal();

	return 0;
}

void speed_changed(int64_t v)
{
	printf("%li (%li.%03lis)\n", v, v / 1000000000L, (v / 1000000) % 1000);
}

