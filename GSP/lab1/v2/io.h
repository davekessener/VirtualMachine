#ifndef DAV_GSP_1_IO_H
#define DAV_GSP_1_IO_H

#include <stdio.h>
#include "common.h"

int lookup(FILE *);
int read_number(double *);

static inline int isnum(int c) { return (c >= '0' && c <= '9') || c == '.'; }
static inline int isws(int c) { return c == '\n' || c == ' ' || c == '\t'; }

#endif

