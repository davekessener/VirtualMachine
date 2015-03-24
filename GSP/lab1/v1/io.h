#ifndef DAV_GSP_1_IO_H
#define DAV_GSP_1_IO_H

#include <stdio.h>
#include "common.h"

/* liesst eine zeile von einem FILE stream */
char *read_line(FILE *);

/* liesst eine decimale zahl, aendert den char ptr dessen
   addresse als argument gegeben wird.
*/
double read_number(const char **);


/* ist 'c' teil einer decimalen zahl [regex: '[0-9]*(\.[0-9]+)?' ] */
static inline int isnum(char c) { return (c >= '0' && c <= '9') || c == '.'; }

/* ist 'c' ein whitespace character */
static inline int isws(char c) { return c == '\n' || c == ' ' || c == '\t'; }

#endif

