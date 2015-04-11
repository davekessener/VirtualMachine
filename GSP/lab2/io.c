#include "io.h"

#define MXT_BUFSIZE 0x40

#define MXT_SETBIT(p,b,v)	p|=(v)<<(b)
#define MXT_GETBIT(p,b)		(((p)>>(b))&1)

void io_init(void)
{
	Init_TI_Board();
}

void io_set_counter(uint c)
{
	MXT_SETBIT(MXT_PORT_COUNT, MXT_BIT_COUNT, c);
}

void io_set_fwd_led(int fwd)
{
	MXT_SETBIT(MXT_PORT_LED, MXT_BIT_LED, fwd);
}

void io_set_rotation(int r)
{
	int s = r < 0 ? -r : r;
	int i = s / MXT_SHIFT;
	int d = s % MXT_SHIFT;
	char buf[MXT_BUFSIZE];

	snprintf(buf, MXT_BUFSIZE, "%c%d.%d", r < 0 ? '-' : ' ', i, d);
	buf[MXT_BUFSIZE - 1] = '\0';

	TCT_cls();
	TCT_puts(buf);
}

void io_set_error(int e)
{
	MXT_SETBIT(MXT_PORT_ERROR, MXT_BIT_ERROR, e);
}

int io_get_reset(void)
{
	return MXT_GETBIT(MXT_PORT_RESET, MXT_BIT_RESET);
}

int io_get_error_reset(void)
{
	return MXT_GETBIT(MXT_PORT_ERRRESET, MXT_BIT_ERRRESET);
}

