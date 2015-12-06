#include "header.h"

extern bool_t checkkey(key_t);
extern void setled(led_t, bool_t);
extern void displaytime(time);

extern time_t time;

state_t state_init(void)
{
	if(checkkey(KEY_S7))
	{
		setled(LED_D20, ON);
		time = 0;

		return STATE_RUNNING;
	}
	else
	{
		return STATE_INIT;
	}
}

state_t state_running(void)
{
	state_t r = STATE_RUNNING;

	if(checkkey(KEY_S5))
	{
		setled(LED_D20, OFF);
		time = 0;

		r = STATE_INIT;
	}
	else if(checkkey(KEY_S6))
	{
		setled(LED_D19, ON);

		r = STATE_HOLD;
	}

	displaytime(time);

	return r;
}

state_t state_hold(void)
{
	if(checkkey(KEY_S5))
	{
		setled(LED_D19, OFF);
		setled(LED_D20, OFF);

		displaytime(0);

		return STATE_INIT;
	}
	else if(checkkey(KEY_S7))
	{
		setled(LED_D19, OFF);

		return STATE_RUNNING;
	}
	else
	{
		return STATE_HOLD;
	}
}

