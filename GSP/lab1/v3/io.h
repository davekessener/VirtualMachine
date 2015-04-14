#ifndef DAV_GSP_1_IO_H
#define DAV_GSP_1_IO_H

#include <stdio.h>
#include "common.h"

/* liesst lookup-character aus dem FILE stream */
int lookup(FILE *);
/* liesst eine zahl von stdin und schreibt sie zu 
   dem uebergebenden pointer. */
int read_int(uint *);
int read_number(double *);

int isnum(int);
int isws(int);

#endif

