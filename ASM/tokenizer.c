#include "tokenizer.h"

void  TOKENIZER_init(TOKENIZER *this)
{
	this->src = this->cur = NULL;
}

void  TOKENIZER_set(TOKENIZER *this, const char *src)
{
	int qf = 0, sqf = 0;
	this->src = this->cur = strdup(src);

	while(*this->cur)
	{
		if(*this->cur >= 'A' && *this->cur <= 'Z')
		{
			if(!qf && !sqf)
			{
				*this->cur -= 'A' - 'a';
			}
		}
		else if(*this->cur == '"')
		{
			if(!sqf && (this->cur == this->src || this->cur[-1] != '\\'))
			{
				qf ^= 1;
			}
		}
		else if(*this->cur == '\'')
		{
			if(!qf &&(this->cur == this->src || this->cur[-1] != '\\'))
			{
				sqf ^= 1;
			}
		}

		this->cur++;
	}

	this->cur = this->src;
}

int inline isWS(char c) { return c == ' ' || c == '\t' || c == '\r'; }
int inline isAl(char c) { return (c >= 'a' && c <= 'z') || c == '_' ; }

int isNum(int b, char c)
{
	static const char *digits = (const char *) "0123456789abcdef";

	while(b-- > 0)
	{
		if(digits[b] == c) return 1;
	}

	return 0;
}

char *TOKENIZER_readToken(TOKENIZER *this)
{
	char buf[BUF_SIZE], *tmp;

	if(this->cur == NULL) return NULL;

	while(isWS(*this->cur)) this->cur++;

	if(!*this->cur) return NULL;

	if(isAl(*this->cur))
	{
		return TOKENIZER_readID(this);
	}
	else if(isNum(10, *this->cur))
	{
		return TOKENIZER_readNumber(this);
	}
	else
	{
		switch(*this->cur)
		{
			case '.':
				this->cur++;
				sprintf(buf, ".%s", tmp = TOKENIZER_readID(this));
				free(tmp);
				break;
			case '%':
				this->cur++;
				sprintf(buf, "%%%s", tmp = TOKENIZER_readNumber(this));
				free(tmp);
				break;
            case ':':
                this->cur++;
                sprintf(buf, ":%s", tmp = TOKENIZER_readID(this));
                free(tmp);
                break;
			case '\'':
				this->cur++;
				sprintf(buf, "%d", TOKENIZER_readChar(this));
				assert(*this->cur=='\'');
				this->cur++;
				break;
			case '"':
				this->cur++;
				sprintf(buf, "%d", TOKENIZER_readChar(this));
				if(*this->cur == '"') this->cur++;
				else
				{
					this->cur -= 2;
					this->cur[0] = ',';
					this->cur[1] = '"';
				}
				break;
			default:
				buf[0] = *this->cur++;
				buf[1] = '\0';
				break;
		}
	}

	return strdup(buf);
}

char *TOKENIZER_readID(TOKENIZER *this)
{
	int l;
	char *buf;

	l = 0;
	while(isAl(*this->cur) || isNum(10, *this->cur))
	{
		this->cur++;
		l++;
	}

	buf = malloc(l + 1);
	memcpy(buf, this->cur - l, l);
	buf[l] = '\0';

	return buf;
}

char *TOKENIZER_readNumber(TOKENIZER *this)
{
	char buf[BUF_SIZE];

	sprintf(buf, "%d", evalNumber(&this->cur));

	return strdup(buf);
}

int TOKENIZER_readChar(TOKENIZER *this)
{
	int r, n;
	assert(*this->cur);

	if(*this->cur == '\\')
	{
		switch(*++this->cur)
		{
			case '\'':
			case '"':
			case '?':
			case '\\':
				r = *this->cur;
				break;
			case 'a': r = 7;  break;
			case 'b': r = 8;  break;
			case 't': r = 9;  break;
			case 'n': r = 10; break;
			case 'v': r = 11; break;
			case 'f': r = 12; break;
			case 'r': r = 13; break;
			case 'x':
				sscanf(this->cur + 1, "%2x", &r);
				this->cur += 2;
				break;
			default:
				sscanf(this->cur, "%3o%n", &r, &n);
				this->cur += n - 1;
				break;
		}
	}
	else
	{
		r = *this->cur;
	}

	this->cur++;

	return r;
}

void  TOKENIZER_dispose(TOKENIZER *this)
{
	free(this->src);

	TOKENIZER_init(this);
}

int evalNumber(char **src)
{
	char *line = *src;
	int val = 0;
	int base = 10;
	int f = 0, s = 1, v;

	char c = *line++;

	if(c == '-')
	{
		s = -1;
		c = *line++;
	}

	if(c == '0')
	{
		switch(c = *line++)
		{
			case '\0':
				(*src)++;

				return 0;
			case 'h':
			case 'x':
				c = *line++;
				base = 16;
				break;
			case 'd':
				c = *line++;
				base = 10;
				break;
			case 'q':
				c = *line++;
				base = 4;
				break;
			case 'b':
				c = *line++;
				base = 2;
				break;
			case 'o':
				line++;
			default:
				f = 1;
				base = 8;
				break;
		}
	}

	while(c && isNum(base, c))
	{
		v = c;
		if(c >= '0' && c <= '9') v -= '0';
		if(c >= 'a' && c <= 'f') v -= 'a' - 10;

		assert(v<16);

		val *= base;
		val += v;

		f = 1;
		c = *line++;
	}

	assert(f);

	*src = line - 1;

	return (s * val);
}

