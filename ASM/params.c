#define PARAMS_C
#include "params.h"
#undef PARAMS_C

void readParameter(PARAMS *this, int argc, char **argv)
{
	const char *fn;
	char **cmds;
	PTABLE tbl;
	PTABLE_init(&tbl);

	this->flags   = FLAG_NONE;
	this->ic      = 0;
	this->output  = NULL;
	this->input   = NULL;
	this->inst    = strdup("ins.dat");

#define PADD(n,p) PTABLE_addParameter(&tbl, n[0], n[1], p)
	PADD(paramHelp, PARAM_NONE);
	PADD(paramVerbose, PARAM_NONE);
	PADD(paramObject, PARAM_NONE);
	PADD(paramLink, PARAM_NONE);
	PADD(paramOut, PARAM_ONE);
	PADD(paramIns, PARAM_ONE);
#undef PADD

	PTABLE_read(&tbl, argc, argv);

#define PARG(n) PTABLE_hasArgument(&tbl, n[0])
	if(PARG(paramHelp))
	{
		this->flags |= FLAG_HELP;
	}
	else
	{
		if(PARG(paramVerbose)) this->flags |= FLAG_VERBOSE;
		if(PARG(paramObject)) this->flags |= FLAG_OBJ;
		if(PARG(paramLink)) this->flags |= FLAG_LNK;
		if(PARG(paramOut))
		{
			this->output = strdup(PTABLE_getValue(&tbl, paramOut[0]));
		}
		if(PARG(paramIns))
		{
			free(this->inst);
			this->inst = strdup(PTABLE_getValue(&tbl, paramIns[0]));
		}

		cmds = PTABLE_getValues(&tbl, NULL);

		if(cmd != NULL)
		{
			for(i = 0 ; cmds[i] ; i++);

			if(i > 0)
			{
				this->ic = i;
				this->input = malloc(i * sizeof(char *));

				while(i-- > 0)
				{
					this->input[i] = strdup(cmds[i]);
				}
			}
		}
	}
#undef PARG
}

void disposeParameter(PARAMS *this)
{
	free(this->output);
	free(this->inst);
	while(this->ic-- > 0)
	{
		free(this->input[this->ic]);
	}
	free(this->input);
}

