/* Header for the PTABLE struct that holds the
 * scanned arguments parsed via 'PTABLE_read'.
 * File: parameter.h for parameter.c
 * Daniel Kessener, HAW
 */
#ifndef __PARAMETER_H
#define __PARAMETER_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// Parameter argument count
enum
{
	PARAM_NONE,
	PARAM_ONE,
	PARAM_MANY
};

// Status of read arguments
enum
{
	STATUS_OK,
	STATUS_DUPLICATE,
	STATUS_UNKNOWN
};

// Argument prototype, eg
// For the 'verbose-flag':
// shortName == "v"
// fullName ==  "verbose"
// This would read from the programs
// command line arguments
// '-v' and '--verbose'
// Arguments for the arguments are read as follows:
// -v arg1 arg2 ... argn
// --verbose="arg"
struct __parameter_type
{
	char *shortName, *fullName;
	int argType;
};

// If this is included from 'parameter.c'
#ifdef PARAMETER_C
typedef struct __parameter_type PTYPE;

void PTYPE_init(PTYPE*, const char *, const char *, int);
void PTYPE_dispose(PTYPE*);
#endif

// Struct for a read argument
struct __parameter
{
	char *id;
	char **vals;
	int status;
};

#ifdef PARAMETER_C
typedef struct __parameter PARA;

void PARA_init(PARA*, const char *);
int  PARA_getSize(PARA*);
void PARA_addValue(PARA*, const char *);
void PARA_dispose(PARA*);
#endif

// Full argument table, holding all prototypes
// as well as the read arguments and their parameter
typedef struct
{
	struct __parameter_type *types;
	struct __parameter *params;
	char *name, *fullName;
	int tc, pc;
	int status;
} PTABLE;

void   PTABLE_init(PTABLE*);
void   PTABLE_addParameter(PTABLE*, const char *, const char *, int);
void   PTABLE_read(PTABLE*, int, char **);
int    PTABLE_hasArgument(PTABLE*, const char *);
char  *PTABLE_getValue(PTABLE*, const char *);
char **PTABLE_getValues(PTABLE*, const char *);
void   PTABLE_dispose(PTABLE*);

#ifdef PARAMETER_C
int    PTABLE_getIndex(PTABLE*, const char *);
#endif

#endif

