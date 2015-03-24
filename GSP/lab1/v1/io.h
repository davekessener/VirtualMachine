#ifndef DAV_GSP_1_IO_H
#define DAV_GSP_1_IO_H

#include <stdio.h>
#include "common.h"

char *read_line(FILE *);
double read_number(const char **);

static inline int isnum(char c) { return (c >= '0' && c <= '9') || c == '.'; }
static inline int isws(char c) { return c == '\n' || c == ' ' || c == '\t'; }

#endif

