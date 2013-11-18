#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <string.h>
#include <malloc.h>
#include <assert.h>
#include "include.h"

typedef struct
{
	char *src, *cur;
} TOKENIZER;

void  TOKENIZER_init(TOKENIZER*);
void  TOKENIZER_set(TOKENIZER*, const char *);
char *TOKENIZER_readToken(TOKENIZER*);
char *TOKENIZER_readID(TOKENIZER*);
char *TOKENIZER_readNumber(TOKENIZER*);
int TOKENIZER_readChar(TOKENIZER*);
void  TOKENIZER_dispose(TOKENIZER*);

int evalNumber(char **);

#endif

