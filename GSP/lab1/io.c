#include "io.h"
#include <malloc.h>
#include <string.h>
#include <assert.h>

/* char buffer fuer dynamisches auslesen,
   insertion verhaelt sich wie eine queue
*/
typedef struct
{
	char *buf;
	uint i, c;
} cbuf_t;

void cbuf_init(cbuf_t *);
void cbuf_push(cbuf_t *, char);

/* free's den buffer und return's eine
   heap-allocated copy des strings im buffer
*/
char *cbuf_str(cbuf_t *);

// # ---------------------------------------------------------------------------

char *read_line(FILE *f)
{
	cbuf_t buf;
	int c;

	cbuf_init(&buf);

	/* liess character bis eol, eos oder eof und insert ihn in den buffer*/
	while((c = getc(f)) != '\n' && c != '\0' && c != EOF)
	{
		cbuf_push(&buf, c);
	}

	return cbuf_str(&buf);
}

// # ---------------------------------------------------------------------------

/* liesst einen integer [regex '[0-9]+']
   sonderfall: beginnt der input mit '.'
   wird null inferiert (so dass '.31' == '0.31')
*/
unsigned long read_int(const char **s)
{
	assert(isnum(**s));

	if(**s == '.') return 0;

	unsigned long r = 0;

	while(**s >= '0' && **s <= '9')
	{
		r = 10 * r + (*(*s)++ - '0');
	}

	return r;
}

// # ---------------------------------------------------------------------------

/* liesst eine zahl in zwei stufen aus:
   integer-teil 'i' zuerst, dann (wenn ein '.' vorliegt)
   den decimalteil 'd'. returned wird die summe i + d
*/
double read_number(const char **s)
{
	unsigned long i = read_int(s);
	double d = 0.0;

	if(**s == '.')
	{
		++*s;
		d = read_int(s);
		while(d >= 1.0) d *= 0.1; /* shifte den decimalteil unter null */
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
	/* wenn der buffer voll ist, reallociere mit verdoppelung + 1 des
	   urspruenglichen speicherplatzes.
	*/
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
		r = malloc((b->i + 1) * sizeof(char)); /* allociere speicherplatz */
		memcpy(r, b->buf, b->i); /* copiere string */
		r[b->i] = '\0'; /* insert eos/null-terminator symbol */

		free(b->buf);
	}

	return r;
}

