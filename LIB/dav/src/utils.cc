#include "utils.h"
#include <aux>

namespace dav { namespace utils {

const std::string alpha("aAbBcCdDeEfFgGhHiIjJkKlLmMnNoOpPqQrRsStTuUvVwWxXyYzZ");
const std::string numerical("0123456789");
const std::string non_alphanum("`~!@#$%^&*()-_=+[{]};:'\"\\|,<.>/? \t\n");
const std::string whitespace(" \t\n");

std::string next(std::string& in, check_fn breakon, check_fn ignore, check_fn contspecial)
{
	std::string out;
	auto i = in.cbegin();
	bool quote = false, br = false, special = false;
	int squote = 0;

	while(*i && ignore(*i)) ++i;

	while(*i && (!br || special))
	{
		if(*i == '\\')
		{
			if(!*++i) throw std::string("ill-formed escape sequence: unexpected eos");
			switch(*i)
			{
				case '0': throw std::string("'\\0' not allowed!");
				case 'n': out.push_back('\n'); break;
				case 't': out.push_back('\t'); break;
				default:  out.push_back(*i);   break;
			}
			++i;
			continue;
		}
		if(quote)
		{
			if(*i == '"')
			{
				quote = false;
				br = true;
			}

			out.push_back(*i);
			++i;
			continue;
		}
		else if(squote)
		{
			if(*i == '\'')
			{
				if(out.length() - squote != 1) throw std::string("malformed single quote");
				squote = 0;
			}

			if(squote && (long)out.length() != squote) throw std::string("malformed single quote");
			
			out.push_back(*i);
			++i;
			continue;
		}
		else if(*i == '"')
		{
			if(!out.empty()) break;

			out.push_back(*i);
			++i;
			quote = true;
			continue;
		}
		else if(*i == '\'')
		{
			if(!out.empty()) break;

			out.push_back(*i);
			++i;
			squote = out.length();
			continue;
		}
		else if(breakon(*i))
		{
			br = true;
			special = contspecial(*i);
		}

		if(*i && (out.empty() || !br || (special && contspecial(*i)))) { out.push_back(*i); ++i; }
		else break;
	}

	while(*i && ignore(*i)) ++i;

	if(quote) throw std::string("malformed quote");
	if(squote) throw std::string("malformed single quote");

	in = std::string(i, in.cend());

	return out;
}

long toInt(const std::string& num)
{
#define MXT_ERR throw std::string("malformed number!")
	if(num.empty()) MXT_ERR;

	QWORD ret = 0;
	bool negative = false;
	int base = 0;

	const char *p = num.data(), *e = p + num.length();
	const char chars[] = "fedcba9876543210", *pchars = chars + sizeof(chars) - 1;

	for(const char *i = p ; i != e ; ++i)
	{
		if(*i == '+' || *i == '-')
		{
			if(i != p) MXT_ERR;
			if(*i == '-') negative = true;
		}
		else if(!base)
		{
			if(*i > '0' && *i <= '9')
			{
				base = 10;
				--i;
				continue;
			}

			if(*i == '0')
			{
				if(!*++i) return 0;
			}
			else if(*i != '$') MXT_ERR;

			if(*i >= '0' && *i <= '7')
			{
				base = 8;
				--i;
				continue;
			}

			switch(*i)
			{
				case 'b':
				case 'B':
					base = 2;
					continue;
				case 'q':
				case 'Q':
					base = 4;
					continue;
				case 'o':
				case 'O':
					base = 8;
					continue;
				case 'd':
				case 'D':
					base = 10;
					continue;
				case '$':
				case 'h':
				case 'x':
				case 'X':
					base = 16;
					continue;
				default:
					MXT_ERR;
			}
		}
		else
		{
			int val = isIn(pchars - base, *i);
			if(val < 0) MXT_ERR;
			val = base - val - 1;
			QWORD tmp = ret * base + val;
			if(tmp < ret) throw std::string("number too large!");
			ret = tmp;
		}
	}

	if(negative)
	{
		long v = -static_cast<long>(ret);
		if(v > 0) MXT_ERR;
		return v;
	}
	else
	{
		return static_cast<long>(ret);
	}
#undef MXT_ERR
}

double toDouble(const std::string& number)
{
#define MXT_ERR throw std::string("malformed number!")
	const char *p = number.data(), *e = p + number.length(), *exp = NULL;
	bool dec = false, num = false;

	for(const char *i = p ; i != e ; ++i)
	{
		if(*i == '+' || *i == '-')
		{
			if(i != p && i != exp) MXT_ERR;
		}
		else if(*i == '.')
		{
			if(dec || exp) MXT_ERR;
			dec = true;
			num = false;
		}
		else if(*i == 'e' || *i == 'E')
		{
			if(exp) MXT_ERR;
			exp = i + 1;
			num = false;
		}
		else if(*i < '0' || *i > '9')
		{
			MXT_ERR;
		}
		else
		{
			num = true;
		}
	}

	if(!num) MXT_ERR;

	return dav::aux::lexical_cast<double>(number);
#undef MXT_ERR
}

}}

