#include "speed.h"

int last, pressed;
uint64_t time;
tzm_change_fn on_change;

void tzm_initialize(tzm_change_fn f)
{
	on_change = f;
	last = -1;
	pressed = 0;
}

void tzm_pressed(int c, uint64_t t)
{
	if(c == '\n')
	{
		on_change(pressed > 0 ? (t - time) / pressed : 0L);
		pressed = 0;
		time = t;
	}
	else
	{
		++pressed;

		if(last < 0)
			time = t;
	}

	last = c;
}

void tzm_cleanup(void)
{
}

