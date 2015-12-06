#include "header.h"

extern void tftPut(int, int, int);

void write_2d(char *, int);
void puts(int, int, const char *);

void displaytime(time_t t)
{
	static char disp[] = "  :  :  .  ";
	static time_t time = -1;

	int hs, s, m, h;

	if(t == time) return;

	time = t;

	hs = t % 100; t /= 100;
	s  = t %  60; t /=  60;
	m  = t %  60; t /=  60;
	h  = t %  24; t /=  24;

	write_2d(disp,      h);
	write_2d(disp + 3,  m);
	write_2d(disp + 6,  s);
	write_2d(disp + 9, hs);

	puts(4, 1, "HAW Timer");
	puts(3, 3, disp);
}

void write_2d(char *s, int v)
{
	*s++ = '0' + ((v / 10) % 10);
	*s   = '0' +  (v % 10);
}

void puts(int x, int y, const char *s)
{
	while(*s)
	{
		tftPut(x++, y, *s++);
	}
}

