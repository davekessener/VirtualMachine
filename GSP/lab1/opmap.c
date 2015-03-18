#include "opmap.h"
#include <malloc.h>
#include <assert.h>

uint opmap_find(const opmap_t *m, char k)
{
	uint i;
	for(i = 0 ; i < m->i ; ++i)
	{
		if(m->ids[i] == k) return i + 1;
	}

	return 0;
}

void opmap_init(opmap_t *m)
{
	m->ids = NULL;
	m->ops = NULL;
	m->i = m->c = 0;
}

void opmap_set(opmap_t *m, char k, op_fn v)
{
	uint i = opmap_find(m, k);

	if(i)
	{
		m->ops[i - 1] = v;
	}
	else
	{
		if(m->i == m->c)
		{
			m->ids = realloc(m->ids, (m->c = 2 * m->c + 1) * sizeof(char));
			m->ops = realloc(m->ops, m->c * sizeof(op_fn));
		}

		m->ids[m->i] = k;
		m->ops[m->i++] = v;
	}
}

op_fn opmap_get(const opmap_t *m, char k)
{
	uint i = opmap_find(m, k);

	if(i) return m->ops[i - 1];
	else return NULL;
}

void opmap_delete(opmap_t *m)
{
	free(m->ids);
	free(m->ops);
}


