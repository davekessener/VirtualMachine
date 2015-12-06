#include "header.h"

extern uint16_t *portset, *portclr;

void setled(led_t led, bool_t f)
{
	if(f)
		*portset = led;
	else
		*portclr = led;
}

