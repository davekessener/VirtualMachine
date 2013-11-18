#include <stdio.h>
#include <stdlib.h>
#include "include.h"
#include "params.h"
#include "tokenizer.h"
#include "preprocessor.h"
#include "assembler.h"

FILE *openfout(const char *);
int runAssembler(ASM *, const char *);

int main(int argc, char *argv[])
{
	FILE *fout;
	ASM *assm;
	INS_TABLE instr;
	PARAMS p;
	char name_buf[BUF_SIZE];
	int i, j, offset;
	WORD main_addr[2];

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

	if((p.output == NULL && !(p.flags & FLAG_OBJ)) || 
		p.input == NULL || p.inst == NULL || p.ic <= 0)
	{
		fprintf(stderr, "ERR: An output as well as one or more "
			"input file[s] have to be specified.\nAbort.\n");
		return EXIT_FAILURE;
	}

	INS_readInstructions(&instr, p.inst);

	assm = malloc(p.ic * sizeof(ASM));
	for(i = 0 ; i < p.ic ; i++)
	{
		ASM_init(assm + i);
		assm[i].ins = &instr;

		if(p.flags & FLAG_LNK)
		{
			ASM_load(assm + i, p.input[i]);
		}
		else
		{
			runAssembler(assm + i, p.input[i]);
		}
	}

	if(p.flags & FLAG_OBJ)
	{
		for(i = 0 ; i < p.ic ; i++)
		{
			sprintf(name_buf, "%s.o", p.input[i]);
			fout = openfout(name_buf);

			ASM_writeLinkable(assm + i, fout);

			fclose(fout);
		}
	}
	else
	{
		offset = 2;

		if(p.flags & FLAG_VERB)
		{
			printf("INITIATING PREPROCESSOR...\n");
		}

		for(i = 0 ; i < p.ic ; i++)
		{
			ASM_prepareLinkage(assm + i, offset);
			offset += assm[i].pos + assm[i].org;
		}

		if(p.flags & FLAG_VERB)
		{
			printf("TRYING TO RESOLVE EXTERNALS...\n");
		}

		for(i = 0 ; i < p.ic ; i++)
		{
			SYM_resolveExternals(&assm[i].sym, assm, p.ic);
		}

		if(p.flags & FLAG_VERB)
		{
			printf("SEARCHING MAIN...\n");
		}

		main_addr[0] = 7;
		main_addr[1] = 0;
		for(i = 0 ; i < p.ic ; i++)
		{
			for(j = 0 ; j < assm[i].sym.cs ; j++)
			{
				if(strcmp(assm[i].sym.syms[j].name, ":main") == 0)
				{
					main_addr[1] = assm[i].sym.syms[j].val;
					goto found_main;
				}
			}
		}

		fprintf(stderr, "ERR: No entry point. Please provide label ':main'.\nAbort.\n");
		return EXIT_FAILURE;

		found_main:

		if(p.flags & FLAG_VERB)
		{
			printf("FINALIZING...\n");
		}

		fout = openfout(p.output);

		fwrite(main_addr, sizeof(WORD), 2, fout);

		for(i = 0 ; i < p.ic ; i++)
		{
			ASM_finalize(assm + i);
			ASM_write(assm + i, fout);
		}

		fclose(fout);
	}

	INS_dispose(&instr);

	for(i = 0 ; i < p.ic ; i++)
	{
		ASM_dispose(assm + i);
	}

	return EXIT_SUCCESS;
}

int runAssembler(ASM *assm, const char *fn)
{
	PREPROCESSOR pp;
	char *tmp;
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

