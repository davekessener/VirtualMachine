#include <malloc.h>
#include "heap.h"

struct
{
	struct
	{
		size_t next;
		void *data;
	} *ps;
	size_t i, c, n;
} heap;

void heap_destructor(void);

void heap_init(void)
{
	heap.ps = NULL;
	heap.i = heap.c = heap.n = 0;
	atexit(heap_destructor);
}

void *new(size_t l)
{
	void *buf = NULL;

	if(heap.n == heap.i)
	{
		if(heap.i == heap.c)
		{
			heap.ps = realloc(heap.ps, (heap.c = 2 * heap.c + 1) * sizeof(*heap.ps));
		}

		heap.ps[heap.i].next = heap.i + 1;
		heap.ps[heap.i].data = NULL;

		++heap.i;
	}

	buf = heap.ps[heap.n].data = malloc(l);

	heap.n = heap.ps[heap.n].next;

	return buf;
}

void delete(void *p)
{
	size_t i;

	if(!p) return;

	for(i = 0 ; i < heap.i ; ++i)
	{
		if(heap.ps[i].data == p)
		{
			free(p);
			heap.ps[i].next = heap.n;
			heap.ps[i].data = NULL;
			heap.n = i;
		}
	}
}

void heap_destructor(void)
{
	size_t i;
	for(i = 0 ; i < heap.i ; ++i)
	{
		free(heap.ps[i]);
	}

	free(heap.ps);
}

