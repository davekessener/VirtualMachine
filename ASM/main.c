#include <stdio.h>
#include <stdlib.h>
#include "params.h"
#include "tokenizer.h"
#include "preprocessor.h"
#include "assembler.h"

#define BUF_SIZE 1024

FILE *openfout(const char *);

int main(int argc, char *argv[])
{
	PREPROCESSOR pp;
	ASM assm;
	INS_TABLE instr;
	PARAMS p;
	FILE *fout;
    char *tmp;
	int err;

	PREPROCESSOR_init(&pp);
	ASM_init(&assm);
	INS_init(&instr);

	readParameter(&p, argc, argv);

	if(p.flags & FLAG_HELP)
	{
		printf("Usage: %s input_files [-h] [-v] [-i/l] [-d ins.dat] -o out.bin\n", argv[0]);
		printf("\t'-h': displays help.\n");
		printf("\t'-v': verbose mode.\n");
		printf("\t'-i': only compilation, no linking.\n");
		printf("\t'-l': linking of precompiled files.\n");
		printf("\t'-d': provides file with instructions. 'ins.dat' is standard.\n");
		printf("\t'-o': specifies output file.\n");

		return EXIT_SUCCESS;
	}

	if(p.output == NULL || p.input == NULL || p.inst == NULL || p.ic <= 0)
	{
		fprintf(stderr, "ERR: An output as well as one or more input file[s] have to be specified.\nAbort.\n");
		return EXIT_FAILURE;
	}

	INS_readInstructions(&instr, p.inst);

	assm = malloc(p.ic * sizeof(ASM));
	for(i = 0 ; i < p.ic ; i++)
	{
		ASM_init(assm + i);

		if(p.flags & FLAG_LNK)
		{
			ASM_load(assm + i, p.input[i]);
		}
		else
		{
			runAssembler(assm + i, p.input[i]);
		}
	}


	fout = openfout(p.output);

	//ASM_finalize(&assm);
	//ASM_write(&assm, fout);
	ASM_writeLinkable(&assm, fout);
	ASM_dispose(&assm);

	fclose(fout);

	return EXIT_SUCCESS;
}

int runAssembler(ASM *assm, const char *fn)
{
	PREPROCESSOR pp;
	int err;

	PREPROCESSOR_init(&pp);

	if((err = runPreprocessor(&pp, fn)))
	{
	    fprintf(stderr, "ERR: %d\nAbort.\n", err);
        return EXIT_FAILURE;
	}

	while((tmp = PREPROCESSOR_getNextLine(&pp)) != NULL)
	{
		ASM_parse(assm, tmp);
	}

	PREPROCESSOR_dispose(&pp);

	return assm->error;
}

FILE *openfout(const char *fn)
{
	FILE *r = fopen(fn, "w");

	assert(r);

	return r;
}

