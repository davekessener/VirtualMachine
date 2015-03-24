#ifndef DAV_GSP_1_OPMAP_H
#define DAV_GSP_1_OPMAP_H

#include "common.h"
#include "stack.h"

typedef void (*op_fn)(stack_t *);

typedef struct
{
	char *ids;
	op_fn *ops;
	uint i, c;
} opmap_t;

void opmap_init(opmap_t *);
void opmap_set(opmap_t *, char, op_fn);
op_fn opmap_get(const opmap_t *, char);
void opmap_delete(opmap_t *);

#endif

