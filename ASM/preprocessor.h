#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tokenizer.h"
#include "squeue.h"

#define PP_STATE_OK  (1<<0)
#define PP_STATE_EQ  (1<<1)
#define PP_STATE_DW  (1<<2)
#define PP_STATE_INC (1<<3)
#define PP_STATE_COM (1<<4)
#define PP_STATE_MAC (1<<5)
#define PP_STATE_MCB (1<<6)

#define MAX_RECURSION 32
#define BUF_SIZE 1024

typedef struct
{
	char **ids;
	S_QUE *vals;
	int c;
} EQS;

typedef struct
{
	char *name;
	int argc, c;
	S_QUE content;
} MACRO;

typedef struct
{
	MACRO *macros;
	int c;
} SYMS;

typedef struct
{
	int error, state, c, ic;
	char **lines, **includes;
	S_QUE tque, tokens, tmp;
	EQS eqs;
	SYMS syms;
	MACRO macro;
} PREPROCESSOR;

int runPreprocessor(PREPROCESSOR*, const char *);
void  PREPROCESSOR_init(PREPROCESSOR*);
void  PREPROCESSOR_process(PREPROCESSOR*);
void  PREPROCESSOR_processQueue(PREPROCESSOR*, S_QUE *, size_t);
void  PREPROCESSOR_eval(PREPROCESSOR*, const char *);
void  PREPROCESSOR_include(PREPROCESSOR*, const char *);
void  PREPROCESSOR_pushLine(PREPROCESSOR*);
char *PREPROCESSOR_getNextLine(PREPROCESSOR*);
void  PREPROCESSOR_dispose(PREPROCESSOR*);

void EQS_init(EQS*);
void EQS_set(EQS*, const char *, S_QUE *);
S_QUE *EQS_get(EQS*, const char *);
void EQS_dispose(EQS*);

void SYMS_init(SYMS*);
void SYMS_push(SYMS*, MACRO *);
MACRO *SYMS_findMacro(SYMS*, const char *, int);
void SYMS_dispose(SYMS*);

void MACRO_init(MACRO*);
void MACRO_addContent(MACRO*, const char *);
void MACRO_eval(MACRO*, S_QUE *, S_QUE *);
void MACRO_dispose(MACRO*);

#endif

