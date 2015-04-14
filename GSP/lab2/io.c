#include "io.h"

#define MXT_BUFSIZE 0x40

#define MXT_SETBIT(p,b,v)	p|=(v)<<(b)
#define MXT_GETBIT(p,b)		(((p)>>(b))&1)

#define MXT_BITMASK_COUNT	0x00ff
#define MXT_BITMASK_LED		0x0001
#define MXT_BITMASK_ERROR	0x0001

#define MXT_GENSETBIT(c,v) MXT_SETBIT(MXT_PORT_ ## c, MXT_BIT_ ## c, (v) & MXT_BITMASK_ ## c)
#define MXT_GENGETBIT(c) MXT_GETBIT(MXT_PORT_ ## c, MXT_BIT_ ## c)

void io_init(void)
{
	Init_TI_Board();
}

void io_set_counter(uint c)
{
	MXT_GENSETBIT(COUNT, c);
}

void io_set_fwd_led(int fwd)
{
	MXT_GENSETBIT(LED, fwd);
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
	MXT_GENSETBIT(ERROR, e);
}

int io_get_reset(void)
{
	return MXT_GENGETBIT(RESET);
}

int io_get_error_reset(void)
{
	return MXT_GENGETBIT(ERRRESET);
}

