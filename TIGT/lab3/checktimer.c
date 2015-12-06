#include "header.h"

extern uint16_t *timer;

time_t checktimer(void)
{
	static uint16_t ttime = 0;
	uint16_t t = *timer;
	time_t r = t - ttime;

	ttime = t - (r % 1000);
	r /= 1000;

	return r;
}

