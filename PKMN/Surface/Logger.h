#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <cstdio>
#include <cstdarg>
#include <cstdlib>
#include <mutex>

#ifdef LOGGER_MAIN
#include <ctime>
#include <cassert>
#endif

#define LOG(...) ::Logger::logFrom(__FILE__, __LINE__, __VA_ARGS__)

class Logger
{
	public:
		static Logger& instance( );
		static void log(const char *, ...);
		static void logFrom(const char *, int, const char *, ...);
		void logMsg(const char *, ...);
		void logMsg(const char *, va_list);
		void logMsgFrom(const char *, int, const char *, va_list);
	private:
		static std::string getTime( );
		Logger( );
		Logger(const Logger&);
		Logger(Logger&&);
		~Logger( );
		Logger& operator=(const Logger&);
		Logger& operator=(Logger&&);
		FILE *_f;
		std::mutex mtx;
};

#endif

