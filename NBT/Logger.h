#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <cstdio>
#include <cstdarg>
#include <cstdlib>

#ifdef LOGGER_MAIN
#include <ctime>
#include <cassert>
#endif

class Logger
{
	public:
		static Logger& instance( );
		static void log(const char *, ...);
		void logMsg(const char *, ...);
		void logMsg(const char *, va_list);
	private:
		static std::string getTime( );
		Logger( );
		Logger(const Logger&);
		Logger(Logger&&);
		~Logger( );
		Logger& operator=(const Logger&);
		Logger& operator=(Logger&&);
		FILE *_f;
};

#endif

