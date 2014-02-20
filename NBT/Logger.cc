#define LOGGER_MAIN
#include "Logger.h"
#undef LOGGER_MAIN

Logger& Logger::instance(void)
{
	static Logger l;
	return l;
}

void Logger::log(const char *s, ...)
{
	va_list l;
	va_start(l, s);

	instance().logMsg(s, l);

	va_end(l);
}

void Logger::logMsg(const char *s, ...)
{
	va_list l;
	va_start(l, s);

	logMsg(s, l);

	va_end(l);
}

void Logger::logMsg(const char *s, va_list l)
{
	fprintf(_f, "%s", getTime().c_str());
	vfprintf(_f, s, l);
	putc('\n', _f);
}

// # ---------------------------------------------------------------------------

std::string Logger::getTime(void)
{
	time_t t = time(0);
	struct tm *now = localtime(&t);

	char buf[64];
	sprintf(buf, "[%02d:%02d:%02d] ", now->tm_hour, now->tm_min, now->tm_sec);

	return std::string(buf);
}

Logger::Logger(void)
{
	time_t t = time(0);
	struct tm *now = localtime(&t);

	char buf[128];
	sprintf(buf, "%04d-%02d-%02d_%02d:%02d:%02d.log",
		now->tm_year + 1900, now->tm_mon + 1, now->tm_mday, now->tm_hour, now->tm_min, now->tm_sec);
	
	_f = fopen(buf, "w");

	assert(_f);
}

Logger::~Logger(void)
{
	fclose(_f);
}

