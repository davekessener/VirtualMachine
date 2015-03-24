#include "io.h"
#include <malloc.h>
#include <string.h>
#include <assert.h>

typedef struct
{
	char *buf;
	uint i, c;
} cbuf_t;

void cbuf_init(cbuf_t *);
void cbuf_push(cbuf_t *, char);
char *cbuf_str(cbuf_t *);

// # ---------------------------------------------------------------------------

char *read_line(FILE *f)
{
	cbuf_t buf;
	int c;

	cbuf_init(&buf);

	while((c = getc(f)) != '\n' && c != '\0' && c != EOF)
	{
		cbuf_push(&buf, c);
	}

	return cbuf_str(&buf);
}

// # ---------------------------------------------------------------------------

long read_int(const char **s)
{
	assert(isnum(**s));

	if(**s == '.') return 0;

	long r = 0;

	while(**s >= '0' && **s <= '9')
	{
		r = 10 * r + (*(*s)++ - '0');
	}

	return r;
}

// # ---------------------------------------------------------------------------

double read_number(const char **s)
{
	long i = read_int(s);
	double d = 0.0;

	if(**s == '.')
	{
		++*s;
		d = read_int(s);
		while(d >= 1.0) d *= 0.1;
	}

	return i + d;
}

// # ===========================================================================

void cbuf_init(cbuf_t *b)
{
	b->buf = NULL;
	b->i = b->c = 0;
}

void cbuf_push(cbuf_t *b, char c)
{
	if(b->i == b->c)
	{
		b->buf = realloc(b->buf, (b->c = 2 * b->c + 1) * sizeof(char));
	}

	b->buf[b->i++] = c;
}

char *cbuf_str(cbuf_t *b)
{
	char *r = NULL;

	if(b->buf)
	{
		r = malloc((b->i + 1) * sizeof(char));
		memcpy(r, b->buf, b->i);
		r[b->i] = '\0';

		free(b->buf);
	}

	return r;
}

