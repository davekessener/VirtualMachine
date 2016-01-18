/* Header for the DStack struct. 
 * File: dstack.h for dstack.c
 * Daniel Kessener, HAW
 */
#ifndef __DSTACK_H
#define __DSTACK_H

#include "include.h"

#ifdef DSTACK_C
#include <malloc.h>
#include <assert.h>
#endif

typedef struct __long_double_stack
{
	FQP *data;
	int c, i;
} DS;

void DS_init(DS*);
void DS_push(DS*, FQP);
FQP  DS_pop(DS*);
void DS_dispose(DS*);

#ifdef DSTACK_C
#define DS_INIT 16

void DS_resize(DS*);
#endif

#endif

