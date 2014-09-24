#ifndef DAV_VM_CPU_H
#define DAV_VM_CPU_H

#include "inc.h"
#include <stdio.h>
#include <stdlib.h>

typedef void (*PSet_fn)(WORD);
typedef WORD (*PGet_fn)(void);

extern WORD *cpu_ram;

void cpu_register(WORD, PSet_fn, PGet_fn);
void cpu_run( );
void cpu_interrupt(WORD);
void cpu_fprint(WORD, WORD, FILE *);
void cpu_print(WORD, WORD);
int cpu_isrunning( );

#endif

