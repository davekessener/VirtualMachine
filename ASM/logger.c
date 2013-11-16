#define LOGGER_C
#include "logger.h"
#undef LOGGER_C

void setLogger(FILE *f)
{
	*getLogger() = f;
}

FILE **getLogger(void)
{
	static FILE *f = NULL;
	return &f;
}

