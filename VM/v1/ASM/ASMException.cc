#include <cstdarg>
#include "ASMException.h"

#define MXT_BUFSIZE 4096

ASMException::ASMException(const char *s, ...) : exception()
{
	char buf[MXT_BUFSIZE];

	va_list l;
	va_start(l, s);

	sprintf(buf, s, l);

	msg_ = std::string(buf);

	va_end(l);
}

ASMException::ASMException(const std::string& msg) : exception(), msg_(msg)
{
}

ASMException::~ASMException(void) throw()
{
}

const char *ASMException::what(void) const throw()
{
	return msg_.c_str();
}

