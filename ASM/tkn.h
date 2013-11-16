/* Header for tokenizing routine. It simplifies parsing
 * by separating the input into tokens specified by the
 * parser.
 * File: tkn.h for tokenizer.c
 * Daniel Kessener, HAW
 */
#ifndef __TOKENIZER_H
#define __TOKENIZER_H

#define TAG_NONE	0
#define TAG_TAG		1
#define TAG_VALUE	2
#define T_BUF_SIZE 1024

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "include.h"
#include "tokenizer.h"

// TKN-structure; Main structure that holds the input (src) and has
// two pointers to the current position within the input (cp) and
// to the beginning of the last successfully read token (pp).
typedef struct
{
	char *src;
	char *cp, *pp;
} TKN;

// T-structure; Holds a single read token. Its type is either TAG_TAG
// (a tag provided with the symbol-table could be read and its value
//  is returned in the data union) or TAG_VALUE (a raw number has been
//  read, its value is return in the data union).
typedef struct
{
	union
	{
		int tag;
		FQP val;
	} data;
	int type;
} T;

// TABLE-structure; Symbol-table that will be supplied to TKN_get; it
// contains all meaningful token with associated integet values that
// the tokenizer should be able to read (ie all tokens the parser can process)
typedef struct
{
	char **ids;
	int *vals;
	int c;
} TABLE;

void TKN_init(TKN*);
// Load input
void TKN_load(TKN*, const char *);
// Read the next token from the input
T	 TKN_get(TKN*, TABLE *);
// Push back last successfully read token
void TKN_unget(TKN*);
void TKN_dispose(TKN*);

void TABLE_init(TABLE*);
// Add recognizable token to symbol table
void TABLE_add(TABLE*, const char *, int);
// Check if given string is a recognized token and
// if it is, return its value
int  TABLE_find(TABLE*, const char *);
void TABLE_dispose(TABLE*);

// Remove all whitespace from the supplied string
void deleteWhitespace(char *);

#endif

