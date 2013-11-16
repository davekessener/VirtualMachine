#ifndef __PARAMS_H
#define __PArAMS_H

#include <stdlib.h>
#include "include.h"
#include "parameter.h"
#include "tokenizer.h"

#define FLAG_NONE 0
#define FLAG_HELP (1<<0)
#define FLAG_VERB (1<<1)
#define FLAG_OBJ  (1<<2)
#define FLAG_LNK  (1<<3)
#define FLAG_OUT  (1<<4)
#define FLAG_INS  (1<<5)

#ifdef PARAMS_C
typedef const char const * CSTR;
CSTR paramHelp[] = {"h", "help"};
CSTR paramVerbose[] = {"v", "verbose"};
CSTR paramObject[] = {"i", "object"};
CSTR paramLink[] = {"l", "link"};
CSTR paramOut[] = {"o", "output"};
CSTR paramIns[] = {"d", "instructions"};
#endif

typedef struct
{
	int flags, ic;
	char *output, **input, *inst;
} PARAMS;

void readParameter(PARAMS*, int, char **);
void disposeParameter(PARAMS*);

#endif

