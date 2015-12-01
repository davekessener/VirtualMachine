#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdarg.h>
#include "debug.h"

#define MXT_PRINTBUF (64*1024)

void fatal(const char *s, ...)
{
	va_list va;
	char buf[MXT_PRINTBUF];

	va_start(va, s);

	vsnprintf(buf, MXT_PRINTBUF - 1, s, va);
	buf[MXT_PRINTBUF - 1] = '\0';

	va_end(va);

	perror(buf);
	exit(EXIT_FAILURE);
}

