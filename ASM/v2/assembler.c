#include "assembler.h"

in_fn in;
out_fn out;



void parse(void)
{
}

void assemble(in_fn fin, out_fn fout)
{
	in = fin;
	out = fout;

	parse();
}

