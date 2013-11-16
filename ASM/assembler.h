#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <string.h>
#include <malloc.h>
#include "tokenizer.h"
#include "numeval.h"

#define BUF_SIZE 1024

#define INS_ARG_NONE  0
#define INS_ARG_CONST 1
#define INS_ARG_REG   2
#define INS_ARG_MEM   3

#define SYM_F_NONE 0
#define SYM_F_ABS (1<<0)
#define SYM_F_EXTERN (1<<1)

#define ASM_F_NONE 0
#define ASM_F_ORG (1<<0)

#define SYM_NAME_LEN 48

typedef unsigned short WORD;

typedef struct
{
	char name[SYM_NAME_LEN];
	WORD val;
	int flags;
} SYM;

typedef struct
{
	SYM *syms;
	char **expr;
	WORD *occ;
	int cs, ce;
} SYM_TABLE;

typedef struct
{
	char *name;
	int args[2], op;
} INST;

typedef struct
{
	INST *ins;
	int c;
} INS_TABLE;

typedef struct
{
	int error, pos, org, flags;
	SYM_TABLE sym;
	INS_TABLE *ins;
	WORD buffer[BUF_SIZE];
} ASM;

void ASM_init(ASM*);
void ASM_parse(ASM*, const char *);
void ASM_finalize(ASM*);
long ASM_write(ASM*, FILE *);
long ASM_writeLinkable(ASM*, FILE *);
int  ASM_load(ASM*, const char *);
void ASM_dispose(ASM*);

void SYM_init(SYM_TABLE*);
void SYM_setValue(SYM_TABLE*, const char *, WORD, int);
void SYM_setExpression(SYM_TABLE*, WORD, const char *);
void SYM_replace(SYM_TABLE*, WORD *);
long SYM_write(SYM_TABLE*, FILE *);
long ASM_writeLinkable(ASM*, FILE *);
void SYM_dispose(SYM_TABLE*);

void INS_init(INS_TABLE*);
void INS_readInstructions(INS_TABLE *, const char *);
void INS_push(INS_TABLE*, INST *);
void INS_dispose(INS_TABLE*);

void S_init(SYM*);
void S_dispose(SYM*);

char *replaceCurPos(const char *, WORD);
char *readMemory(const char *, int *);

#endif

