#ifndef SQUEUE_H
#define SQUEUE_H

#include <string.h>
#include <assert.h>
#include <malloc.h>
#include "include.h"

typedef struct
{
	char **data;
	int c, i;
} S_QUE;

void SQ_init(S_QUE*);
void SQ_push(S_QUE*, const char *);
const char *SQ_poll(S_QUE*);
char *SQ_join(S_QUE*);
void SQ_dispose(S_QUE*);

#endif

