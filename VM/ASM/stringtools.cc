#include <cstdio>
#include <cstdarg>
#include <cstdlib>
#include "stringtools.h"

#define MXT_BUFSIZE 4096

void trim(std::string& s)
{
	std::string r, t;
	bool beg = false;

	for(auto i = s.cbegin() ; i != s.cend() ; ++i)
	{
		switch(*i)
		{
			case ' ':
			case '\t':
			case '\n':
			case '\0':
				if(beg)
				{
					t.push_back(*i);
				}
				break;
			default:
				r += t;
				r.push_back(*i);
				t.clear();
				beg = true;
				break;
		}
	}

	s = r;
}

std::string string_format(const char *s, ...)
{
	va_list l;
	va_start(l, s);

	char buf[MX_BUFSIZE];

	vsprintf(buf, s, l);

	va_end(l);

	return std::string(buf);
}

