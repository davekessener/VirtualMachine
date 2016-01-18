#ifndef ASSEMBLER_H
#define ASSEMBLER_H

typedef char (*in_fn)(void);
typedef void (*out_fn)(char);

void assemble(in_fn, out_fn);

#endif

