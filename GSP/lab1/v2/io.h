#ifndef DAV_GSP_1_IO_H
#define DAV_GSP_1_IO_H

#include <stdio.h>
#include "common.h"

/* liesst lookup-character aus dem FILE stream */
int lookup(FILE *);
/* liesst einen double von stdin und schreibt ihn zu 
   dem uebergebenden pointer. */
int read_number(double *);

static inline int isnum(int c) { return (c >= '0' && c <= '9') || c == '.'; }
static inline int isws(int c) { return c == '\n' || c == ' ' || c == '\t'; }

#endif

