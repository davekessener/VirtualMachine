#include "Geometry.h"

std::string format(const char *s, ...)
{
	if(!s||!*s)
	{
		return std::string(SDL_GetError());
	}

	va_list l;
	va_start(l, s);

	char buf[2048];

	vsprintf(buf, s, l);

	va_end(l);

	return std::string(buf);
}

