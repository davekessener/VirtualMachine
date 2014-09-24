#ifndef LOGGER_H
#define LOGGER_H

#include <string>

#ifdef MXT_MT_CAPABLE
#include <mutex>
#endif

#define LOG(...) ::Logger::logFrom(__FILE__, __LINE__, __VA_ARGS__)

class Logger
{
	public:
		static Logger& instance( );
		static const std::string log(const char *, ...);
		static const std::string log(const std::string&);
		static const std::string logFrom(const char *, int, const char *, ...);
		const std::string logMsg(const char *, ...);
		const std::string logMsg(const char *, va_list);
		const std::string logMsgFrom(const char *, int, const char *, va_list);
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

