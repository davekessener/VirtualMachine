#include "preprocessor.h"

int runPreprocessor(PREPROCESSOR *this, const char *src)
{
	PREPROCESSOR_include(this, src);

	return this->error;
}

void  PREPROCESSOR_init(PREPROCESSOR *this)
{
	this->error = 0;
	this->state = PP_STATE_OK;
	this->c = this->ic = 0;
	this->lines = this->includes = NULL;

	SQ_init(&this->tque);
	SQ_init(&this->tokens);
	SQ_init(&this->tmp);

	EQS_init(&this->eqs);
	SYMS_init(&this->syms);

	MACRO_init(&this->macro);
}

void  PREPROCESSOR_process(PREPROCESSOR *this) { PREPROCESSOR_processQueue(this, &this->tokens, 0); }
void  PREPROCESSOR_processQueue(PREPROCESSOR *this, S_QUE *queue, size_t rLevel)
{
	const char *token;
	S_QUE *tmp;

	assert(rLevel<MAX_RECURSION);

	while(queue->i < queue->c)
	{
		token = SQ_poll(queue);

		if((tmp = EQS_get(&this->eqs, token)))
		{
			PREPROCESSOR_processQueue(this, tmp, rLevel + 1);
			tmp->i = 0;
		}
		else
		{
			PREPROCESSOR_eval(this, token);
		}

		if(this->error) break;
	}
}

void PREPROCESSOR_eval(PREPROCESSOR *this, const char *token)
{
#define CHKSTATE(s) (this->state&(s))
#define ISNL (*token=='\n')
	char *tmp;

	if(this->error) return;

	if(CHKSTATE(PP_STATE_COM))
	{
	    if(ISNL)
	    {
	        this->state &= ~PP_STATE_COM;
	    }
	    else
	    {
	        return;
	    }
	}

	if(CHKSTATE(PP_STATE_EQ))
	{
		if(ISNL)
		{
			if(this->tmp.c <= 0 || this->tque.c <= 0)
			{
				this->error |= PP_STATE_EQ;
				return;
			}
			else
			{
				EQS_set(&this->eqs, tmp = SQ_join(&this->tmp), &this->tque);
				free(tmp);
				SQ_dispose(&this->tmp);
				SQ_init(&this->tque);
			}

			this->state &= ~PP_STATE_EQ;
		}
		else
		{
			if(this->tmp.c <= 0)
			{
				SQ_push(&this->tmp, token);
			}
			else
			{
				SQ_push(&this->tque, token);
			}
		}
	}
	else if(CHKSTATE(PP_STATE_INC))
	{
		if(ISNL)
		{
	        this->state &= ~PP_STATE_INC;
	        PREPROCESSOR_include(this, tmp = SQ_join(&this->tmp));
			free(tmp);
			SQ_dispose(&this->tmp);
		}
		else
		{
			if(this->tmp.c > 0)
			{
				tmp = malloc(strlen(this->tmp.data[0]) + strlen(token) + 1);
				strcpy(tmp, *this->tmp.data);
				strcat(tmp, token);
			}
			else
			{
				tmp = strdup(token);
			}

			SQ_dispose(&this->tmp);
			SQ_push(&this->tmp, tmp);
		}
	}
	else if(CHKSTATE(PP_STATE_DW))
	{
		if(strcmp(token, ",") == 0 || ISNL)
		{
			PREPROCESSOR_pushLine(this);
		}
		else
		{
			if(this->tmp.c <= 0)
			{
				SQ_push(&this->tmp, ".dw");
			}

			SQ_push(&this->tmp, token);
		}

		if(ISNL)
		{
			this->state &= ~PP_STATE_DW;
		}
	}
	else if(CHKSTATE(PP_STATE_MAC))
	{
		if(CHKSTATE(PP_STATE_MCB))
		{
			if(strcmp(token, "{") == 0)
			{
				this->state &= ~PP_STATE_MCB;
			}
			else
			{
				if(this->macro.name == NULL)
				{
					this->macro.name = strdup(token);
				}
				else
				{
					sscanf(token, "%d", &this->macro.argc);
				}
			}
		}
		else if(strcmp(token, "}") == 0)
		{
			SYMS_push(&this->syms, &this->macro);
			MACRO_init(&this->macro);

			this->state &= ~PP_STATE_MAC;
		}
		else
		{
			MACRO_addContent(&this->macro, token);
		}
	}
	else
	{
		if(ISNL)
		{
			PREPROCESSOR_pushLine(this);
		}
		else if(strcmp(token, ".equ") == 0)
		{
			this->state |= PP_STATE_EQ;
		}
		else if(strcmp(token, ".dw") == 0 || strcmp(token, ".db") == 0)
		{
			this->state |= PP_STATE_DW;
		}
        else if(strcmp(token, ".inc") == 0)
        {
            this->state |= PP_STATE_INC;
        }
		else if(strcmp(token, ".mac") == 0)
		{
			this->state |= PP_STATE_MAC;
			this->state |= PP_STATE_MCB;
		}
	    else if(strcmp(token, ";") == 0)
	    {
	        this->state |= PP_STATE_COM;
	    }
		else
		{
			SQ_push(&this->tmp, token);
		}
	}
#undef CHKSTATE
#undef ISNL
}

void PREPROCESSOR_include(PREPROCESSOR *this, const char *src)
{
	TOKENIZER t;
	S_QUE sq;
	FILE *inc;
	char buf[BUF_SIZE], *tmp;
	long s;
	int i;

	assert(*src);

	for(i = 0 ; i < this->ic ; i++)
	{  
		if(strcmp(this->includes[i], src) == 0) return;
	}

//	fprintf(stderr, "@OPENING '%s'\n", src);
	inc = fopen(src, "rb");

	assert(inc);

	this->includes = realloc(this->includes, ++this->ic * sizeof(char *));
	this->includes[this->ic - 1] = strdup(src);

	fseek(inc, 0, SEEK_END);
	s = ftell(inc);
	fseek(inc, 0, SEEK_SET);

	assert(s<BUF_SIZE);

	fread(buf, 1, s, inc);
	buf[s] = '\0';

//	fprintf(stderr, "@CLOSING '%s'\n", src);
	fclose(inc);

	SQ_init(&sq);
	TOKENIZER_init(&t);

	TOKENIZER_set(&t, buf);

	while((tmp = TOKENIZER_readToken(&t)) != NULL)
	{
	    SQ_push(&sq, tmp);
	    free(tmp);
	}

	TOKENIZER_dispose(&t);

	PREPROCESSOR_processQueue(this, &sq, 0);

	SQ_dispose(&sq);
}

void PREPROCESSOR_pushLine(PREPROCESSOR *this)
{
	const char *name, *tmp;
	S_QUE *args = NULL, eval;
	MACRO *m;
	int c = 0;

	if(this->tmp.c <= 0) return;

	SQ_init(&eval);

	this->tmp.i = 0;
	name = SQ_poll(&this->tmp);

	while((tmp = SQ_poll(&this->tmp)) != NULL)
	{
		if(c == 0)
		{
			args = malloc(sizeof(S_QUE));
			SQ_init(args);
			c = 1;
		}

		if(strcmp(tmp, ",") == 0)
		{
			args = realloc(args, ++c * sizeof(S_QUE));
			SQ_init(&args[c - 1]);
		}
		else
		{
			SQ_push(&args[c - 1], tmp);
		}
	}

	m = SYMS_findMacro(&this->syms, name, c);

	if(m != NULL)
	{
		MACRO_eval(m, &eval, args);	
		
		SQ_dispose(&this->tmp);

		PREPROCESSOR_processQueue(this, &eval, 0);

		SQ_dispose(&eval);
	}
	else
	{
		this->tmp.i = 0;
		this->lines = realloc(this->lines, ++this->c * sizeof(char *));
		this->lines[this->c - 1] = SQ_join(&this->tmp);
		SQ_dispose(&this->tmp);
	}

	while(c-- > 0)
	{
		SQ_dispose(&args[c]);
	}

	free(args);

	SQ_dispose(&eval);
}

char *PREPROCESSOR_getNextLine(PREPROCESSOR *this)
{
	char *tmp = NULL;

	if(this->c > 0)
	{
		tmp = this->lines[0];
		memmove(this->lines, this->lines + 1, --this->c * sizeof(char *));

		if(this->c == 0)
		{
			free(this->lines);
			this->lines = NULL;
		}
		else
		{
			this->lines = realloc(this->lines, this->c * sizeof(char *));
		}
	}

	return tmp;
}

void  PREPROCESSOR_dispose(PREPROCESSOR *this)
{
	while(this->c-- > 0)
	{
		free(this->lines[this->c]);
	}
	free(this->lines);

	while(this->ic-- > 0)
	{
		free(this->includes[this->ic]);
	}
	free(this->includes);

	SQ_dispose(&this->tque);
	SQ_dispose(&this->tokens);

	EQS_dispose(&this->eqs);
	SYMS_dispose(&this->syms);

	MACRO_dispose(&this->macro);

	PREPROCESSOR_init(this);
}

// # ===========================================================================

void EQS_init(EQS *this)
{
	this->ids = NULL;
	this->vals = NULL;
	this->c = 0;
}

int EQS_getPos(EQS *this, const char *id)
{
	int i;
	for(i = 0 ; i < this->c ; i++)
	{
		if(strcmp(this->ids[i], id) == 0) return i;
	}

	return -1;
}

void EQS_set(EQS *this, const char *id, S_QUE *val)
{
	int i = EQS_getPos(this, id);

	if(i < 0)
	{
		i = this->c++;

		this->ids = realloc(this->ids, this->c * sizeof(char *));
		this->vals = realloc(this->vals, this->c * sizeof(S_QUE));

		this->ids[i] = strdup(id);
		SQ_init(&this->vals[i]);
	}

	SQ_dispose(&this->vals[i]);
	memcpy(&this->vals[i], val, sizeof(S_QUE));
}

S_QUE *EQS_get(EQS *this, const char *name)
{
	int i = EQS_getPos(this, name);

	return i < 0 ? NULL : &this->vals[i];
}

void EQS_dispose(EQS *this)
{
	while(this->c-- > 0)
	{
		free(this->ids[this->c]);
		SQ_dispose(&this->vals[this->c]);
	}

	free(this->ids);
	free(this->vals);

	EQS_init(this);
}

// # ---------------------------------------------------------------------------

void SYMS_init(SYMS *this)
{
	this->macros = NULL;
	this->c = 0;
}

void SYMS_push(SYMS *this, MACRO *m)
{
	this->macros = realloc(this->macros, ++this->c * sizeof(MACRO));
	memcpy(this->macros + this->c - 1, m, sizeof(MACRO));
}

MACRO *SYMS_findMacro(SYMS *this, const char *name, int argc)
{
	int i;
	for(i = 0 ; i < this->c ; i++)
	{
		if(strcmp(this->macros[i].name, name) == 0 && this->macros[i].argc == argc)
		{
			return &this->macros[i];
		}
	}

	return NULL;
}

void SYMS_dispose(SYMS *this)
{
	while(this->c-- > 0)
	{
		MACRO_dispose(&this->macros[this->c]);
	}

	free(this->macros);

	SYMS_init(this);
}

// # ---------------------------------------------------------------------------

void MACRO_init(MACRO *this)
{
	this->name = NULL;
	this->argc = 0;
	this->c    = 0;
	
	SQ_init(&this->content); 
}

void MACRO_addContent(MACRO *this, const char *cont)
{
	SQ_push(&this->content, cont);
}

void MACRO_eval(MACRO *this, S_QUE *dst, S_QUE *src)
{
	int i;
	const char *tmp;
	char buf[BUF_SIZE];

	assert(this->argc>=0);

	this->content.i = 0;
	while((tmp = SQ_poll(&this->content)) != NULL)
	{
		if(*tmp == '%')
		{
			if(sscanf(tmp + 1, "%d", &i) != 1)
			{
				fprintf(stderr, "ERR: Invalid read of '%s'\nAbort.\n", tmp);
				exit(1);
			}

			assert(i>=0&&i<this->argc);

			src[i].i = 0;
			while((tmp = SQ_poll(&src[i])) != NULL)
			{
				SQ_push(dst, tmp);
			}
		}
		else if(*tmp == ':')
		{
			sprintf(buf, "%s_%s_%d_%d", tmp, this->name, this->argc, this->c);
			SQ_push(dst, buf);
		}
		else
		{
			SQ_push(dst, tmp);
		}
	}

	this->c++;
}

void MACRO_dispose(MACRO *this)
{
	free(this->name);

	SQ_dispose(&this->content);

	MACRO_init(this);
}

