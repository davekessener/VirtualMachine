#ifndef LOGGER_H
#define LOGGER_H

#include <string>

#ifdef MXT_MT_CAPABLE
#include <mutex>
#endif

#ifndef NDEBUG
#define LOG(...) ::Logger::logFrom(__FILE__, __LINE__, __VA_ARGS__)
#else
#define LOG(...)
#endif

class Logger
{
	public:
		static Logger& instance( );
		static void log(const char *, ...);
		static void log(const std::string&);
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
#ifdef MXT_MT_CAPABLE
		std::mutex mtx;
#endif
};

#endif

