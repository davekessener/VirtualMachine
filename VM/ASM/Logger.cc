#include "Logger.h"
#include <iostream>
#include <cstdio>
#include <cstdarg>
#include <cstdlib>
#include <ctime>
#include <cassert>

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

void Logger::log(const std::string& msg)
{
	instance().logMsg("%s", msg.c_str());
}

void Logger::logFrom(const char *f, int i, const char *s, ...)
{
	va_list l;
	va_start(l, s);

	instance().logMsgFrom(f, i, s, l);

	va_end(l);
}

void Logger::logMsg(const char *s, ...)
{
#ifdef MXT_MT_CAPABLE
	mtx.lock();
#endif

	va_list l;
	va_start(l, s);

	logMsg(s, l);

	va_end(l);

#ifdef MXT_MT_CAPABLE
	mtx.unlock();
#endif
}

void Logger::logMsg(const char *s, va_list l)
{
#ifdef MXT_MT_CAPABLE
	mtx.lock();
#endif

	fprintf(_f, "[%s] ", getTime().c_str());
	vfprintf(_f, s, l);
	putc('\n', _f);
	fflush(_f);

#ifdef MXT_MT_CAPABLE
	mtx.unlock();
#endif
}

void Logger::logMsgFrom(const char *f, int i, const char *s, va_list l)
{
#ifdef MXT_MT_CAPABLE
	mtx.lock();
#endif

	fprintf(_f, "[%s] ", getTime().c_str());
	fprintf(_f, "[%s:%d] ", f, i);
	vfprintf(_f, s, l);
	putc('\n', _f);
	fflush(_f);

#ifdef MXT_MT_CAPABLE
	mtx.unlock();
#endif
}

// # ---------------------------------------------------------------------------

std::string Logger::getTime(void)
{
	time_t t = time(0);
	struct tm *now = localtime(&t);

	char buf[64];
	sprintf(buf, "%02d:%02d:%02d", now->tm_hour, now->tm_min, now->tm_sec);

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

