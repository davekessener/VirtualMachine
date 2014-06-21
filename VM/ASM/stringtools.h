#ifndef STRINGTOOLS_H
#define STRINGTOOLS_H

#include <string>

#define MXT_ACCEPTABLEID "_%:"

namespace stringtools
{
	void trim(std::string&);
	std::string string_format(const char *, ...);
	const std::string token_extract(std::string&, int&);
	int convert_character(const char *&);
	int getBase(char);
	
	inline bool isWS(char c) { return c == ' ' || c == '\t'; }
	inline bool isIn(char c, const char *s) { while(*s) if(*s++ == c) return true; return false; }
	inline bool isNO(char c, int base = 10) 
	{
		static const char *d = "fedcba9876543210";
		return isIn((c >= 'A' && c <= 'F') ? (c - 'A' + 'a') : c, d + 16 - base);
	}
	inline bool isID(char c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || isNO(c) || isIn(c, MXT_ACCEPTABLEID); }
}

#endif

