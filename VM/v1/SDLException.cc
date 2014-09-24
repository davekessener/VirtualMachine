#include "SDLException.h"
#include <cstdarg>

SDLException::SDLException(void) : exception(), msg(SDL_GetError())
{
}

SDLException::SDLException(const char *s, ...) : exception()
{
	char buf[1024];

	va_list l;
	va_start(l, s);

	sprintf(buf, s, l);

	msg = std::string(buf);

	va_end(l);
}

SDLException::SDLException(const std::string& _msg) : exception(), msg(_msg)
{
}

SDLException::~SDLException(void) throw()
{
}

const char *SDLException::what(void) const throw()
{
	return msg.c_str();
}

