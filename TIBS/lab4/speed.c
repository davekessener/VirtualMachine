#include "speed.h"

int last, pressed;
int64_t def;
uint64_t time;
tzm_change_fn on_change;

void tzm_initialize(int64_t d, tzm_change_fn f)
{
	def = d;
	on_change = f;
	last = -1;
	pressed = 0;
}

void tzm_pressed(int c, uint64_t t)
{
	if(c == '\n')
	{
		on_change(pressed > 0 ? (t - time) / pressed : def);
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

