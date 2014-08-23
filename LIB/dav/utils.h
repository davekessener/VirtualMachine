#ifndef DAV_UTILS_H
#define DAV_UTILS_H

#include <dav/inc.h>
#include <string>
#include <functional>

namespace dav
{
	namespace utils
	{
		typedef std::function<bool(char)> check_fn;

		extern const std::string alpha;
		extern const std::string numerical;
		extern const std::string non_alphanum;
		extern const std::string whitespace;

		inline int isIn(const std::string& s, char c) { return s.find(c); }
		inline int isIn(const char *s_, char c)
			{ for(const char *s = s_ ; *s ; ++s) if(*s == c) return s - s_; return -1; }

		inline bool std_breakon(char c) {  return isIn(non_alphanum, c) != -1; }
		inline bool std_ignore(char c) { return isIn(whitespace, c) != -1; }
		inline bool std_contspecial(char c) { return false; }

		std::string next(std::string&, check_fn = std_breakon, check_fn = std_ignore, check_fn = std_contspecial);
		long toInt(const std::string&);
		double toDouble(const std::string&);
	}
}

#endif

