#include "ASMException.h"
#include <cstdarg>

#define MXT_BUFSIZE 4096

ASMException::ASMException(const char *s, ...) : exception()
{
	char buf[MXT_BUFSIZE];

	va_list l;
	va_start(l, s);

	sprintf(buf, s, l);

	msg = std::string(buf);

	va_end(l);
}

ASMException::ASMException(const std::string& _msg) : exception(), msg(_msg)
{
}

ASMException::~ASMException(void) throw()
{
}

const char *ASMException::what(void) const throw()
{
	return msg.c_str();
}

