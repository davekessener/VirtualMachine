#include <cstdio>
#include <cstdarg>
#include <cstdlib>
#include <cassert>
#include "stringtools.h"

#define MXT_BUFSIZE 4096

namespace stringtools {

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

	char buf[MXT_BUFSIZE];

	vsprintf(buf, s, l);

	va_end(l);

	return std::string(buf);
}

int getBase(char c)
{
	if(c >= '1' && c <= '7')
		return 8;
	else switch(c)
	{
		case 'b':
		case 'B':
			return 2;
		case 'o':
		case 'O':
			return 8;
		case 'd':
		case 'D':
			return 10;
		case 'h':
		case 'x':
		case 'X':
			return 16;
		default:
			throw std::string("ERR: Malformed number!");
	}

	assert(!"Should never be here!");

	return 10;
}

const std::string token_extract(std::string& in_s, int& offset)
{
	auto s = in_s.begin();

	while(isWS(*s))
	{
		++offset;
		++s;
	}

	if(!*s) return in_s = std::string("");

	auto beg = s;
	bool quote = false, skip = false, id = false, num = false;
	int base = 0, sq = 0;

	while(*s && (quote || sq || !isWS(*s)))
	{
		if(isWS(*s)) *s = ' ';

		if(sq)
		{
			if(sq == 1)
			{
				if(skip)
				{
					skip = false;
					++sq;
				}
				else if(*s == '\'')
				{
					throw std::string("ERR: malformed(empty) sigle quote");
				}
				else if(*s == '\\')
				{
					skip = true;
				}
				else
				{
					++sq;
				}
			}
			else
			{
				if(*s != '\'')
				{
					throw std::string("ERR: malformed(too long) single quote");
				}
				else
				{
					++s;
					sq = 0;
					break;
				}
			}
		}
		else if(quote)
		{
			if(skip)
			{
				skip = false;
			}
			else if(*s == '\\')
			{
				skip = true;
			}
			else if(*s == '"')
			{
				quote = false;
				++s;
				break;
			}
		}
		else if(id)
		{
			if(!isID(*s)) break;
		}
		else if(num)
		{
			if(!base)
			{
				base = getBase(*s);
			}
			else if(!isNO(*s, base))
			{
				break;
			}
		}
		else if(*s == '"')
		{
			quote = true;
		}
		else if(*s == '\'')
		{
			sq = 1;
		}
		else if(isID(*s) || *s == '.')
		{
			id = true;
		}
		else if(isNO(*s))
		{
			num = true;
			if(*s != '0') base = 10;
		}
		else if(isIn(*s, ";&|$%<>=!"))
		{
			char t(*s);
			if(t == *++s) ++s;
			else switch(t)
			{
				case '!':
				case '>':
				case '<':
					if(*s == '=') ++s;
					break;
			}

			break;
		}
		else
		{
			++s;
			break;
		}

		++s;
	}

	if(quote && !*s)
	{
		throw std::string("ERR: malformed(premature) string");
	}
	else if(sq && !*s)
	{
		throw std::string("ERR: malformed(premature) single quote");
	}

	std::string r(beg, s);
	in_s = std::string(s, in_s.end());

	return r;
}

int convert_character(const char *& s)
{
	int r = *s;

	if(*s == '\t')
	{
		r = ' ';
	}
	else if(*s == '\\')
	{
		switch(*++s)
		{
			case '0':  r = '\0'; break;
			case 'b':  r = '\b'; break;
			case 'r':  r = '\r'; break;
			case '\'': r = '\''; break;
			case '"':  r = '\"'; break;
			case 'n':  r = '\n'; break;
			case 't':  r = '\t'; break;
			case '\\': r = '\\'; break;
			default:
				throw string_format("ERR: Invalid character after '\\': '%c'", *s);
		}
	}

	++s;

	return r;
}

}

