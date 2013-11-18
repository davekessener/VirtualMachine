#include "assembler.h"

void ASM_init(ASM *this)
{
	this->pos   = this->org = 0;
	this->error = 0;
	this->flags = ASM_F_NONE;
	this->ins   = NULL;
	this->cpc   = 0;
	
	SYM_init(&this->sym);
}

void ASM_parse(ASM *this, const char *line)
{
	TOKENIZER t;
	INST *inst = NULL;
	char name_buf[BUF_SIZE], arg_buf[BUF_SIZE], buf[BUF_SIZE], *tmp;
	int args[2], i, j, k, curPos;

	assert(this->ins);

	TOKENIZER_init(&t);

	printf("PARSING: %s\n", line);

	if(!line || !*line || *line == '\n')
	{
		return;
	}
	else if(*line == '.')
	{
		if(memcmp(line, ".dw", 3) == 0)
		{
			//sscanf(line + 3, " %d", &i);
			SYM_setExpression(&this->sym, this->pos, line + 4);
//			i = executeNumberEvaluator(line + 4);
			this->buffer[this->pos++] = 0;
		}
		else if(memcmp(line, ".org", 4) == 0)
		{
			//sscanf(line + 4, " %d", &i);
			i = executeNumberEvaluator(line + 5);
			this->org = i - this->pos;
			this->flags |= ASM_F_ORG;
		}
		else if(memcmp(line, ".res", 4) == 0)
		{
			i = executeNumberEvaluator(line + 5);
			memset(this->buffer + this->pos, 0, i * sizeof(*this->buffer));
			this->pos += i;
		}
		else if(memcmp(line, ".orst", 5) == 0)
		{
			this->org = 0;
			this->flags &= ~ASM_F_ORG;
		}
		else if(memcmp(line, ".extern", 7) == 0)
		{
			SYM_setValue(&this->sym, line + 8, 0, SYM_F_EXTERN);
		}
		else
		{
			assert(0);
		}
	}
	else if(*line == ':')
	{
		SYM_setValue(&this->sym, line, this->pos + this->org, (this->flags & ASM_F_ORG)  ? SYM_F_ABS : SYM_F_NONE);
	}
	else
	{
		TOKENIZER_set(&t, line);
		memset(arg_buf, 0, BUF_SIZE);

		i = 0;
		while((tmp = TOKENIZER_readToken(&t)) != NULL)
		{
			if(i == 0)
			{
				strncpy(name_buf, tmp, BUF_SIZE);
				i++;
			}
			else
			{
				if(strcmp(tmp, ",") == 0)
				{
					i++;
				}
				else
				{
					strncat(arg_buf + (i - 1) * BUF_SIZE / 2, tmp, BUF_SIZE / 2);
				}
			}

			assert(i<3);

			free(tmp);
		}

		args[0] = args[1] = INS_ARG_NONE;
		for(i = 0 ; i < 2 ; i++)
		{
			switch(arg_buf[i * BUF_SIZE / 2])
			{
				case '\0': break;
				case 'r':  args[i] = INS_ARG_REG;   break;
				case '(':
					free(readMemory(arg_buf + i * BUF_SIZE / 2, &k));
					if(k >= 0)
					{
						args[i] = INS_ARG_MEM;
						break;
					}
				default:   args[i] = INS_ARG_CONST; break;
			}
		}

		for(i = 0 ; i < this->ins->c ; i++)
		{
			if(strcmp(this->ins->ins[i].name, name_buf) == 0)
			{
				if(this->ins->ins[i].args[0] == args[0] && this->ins->ins[i].args[1] == args[1])
				{
					inst = &this->ins->ins[i];
					break;
				}
			}
		}

		if(!inst)
		{
			fprintf(stderr, "ERR: '%s %s%c%s' is not a valid instruction.\nAbort.\n", 
						name_buf, arg_buf, arg_buf[BUF_SIZE / 2] ? ',' : '\0', arg_buf + BUF_SIZE / 2);
			exit(1);
		}

		curPos = this->pos;
		this->buffer[this->pos++] = inst->op;
		for(i = 0 ; i < 2 ; i++)
		{
			switch(inst->args[i])
			{
				case INS_ARG_NONE: break;
				case INS_ARG_CONST:
					sprintf(buf, ":__%d", this->cpc++);
					tmp = replaceCurPos(arg_buf + i * BUF_SIZE / 2, buf);
					SYM_setValue(&this->sym, buf, curPos + this->org, (this->flags & ASM_F_ORG)  ? SYM_F_ABS : SYM_F_NONE);
					SYM_setExpression(&this->sym, this->pos, tmp);
					free(tmp);
					this->buffer[this->pos++] = 0;
					break;
				case INS_ARG_REG:
					sscanf(arg_buf + i * BUF_SIZE / 2, "r%d", &j);
					this->buffer[this->pos++] = j;
					break;
				case INS_ARG_MEM:
					tmp = readMemory(arg_buf + i * BUF_SIZE / 2, &k);
					assert(tmp);
					if(tmp[0] != '0' || tmp[1] != '\0')
					{
						SYM_setExpression(&this->sym, curPos + 1, tmp);
						memmove(this->buffer + curPos + 2, this->buffer + curPos, (i + 1) * sizeof(WORD));
						this->pos += 2;
						for(j = 0 ; j < this->ins->c ; j++)
						{
							if(strcmp(this->ins->ins[j].name, "off") == 0)
							{
								this->buffer[curPos] = this->ins->ins[j].op;
								break;
							}

							assert(j+1<this->ins->c);
						}
					}
					this->buffer[this->pos++] = k;
					free(tmp);
					break;
			}
		}
	}
}

void ASM_prepareLinkage(ASM *this, int offset)
{
	SYM_setOffset(&this->sym, offset);
}

void ASM_finalize(ASM *this)
{
	SYM_replace(&this->sym, this->buffer);
}

long ASM_write(ASM *this, FILE *fout)
{
	fwrite(this->buffer, sizeof(*this->buffer), this->pos, fout);
	return this->pos;
}

long ASM_writeLinkable(ASM *this, FILE *fout)
{
	fwrite(&this->pos, sizeof(int), 1, fout);
	fwrite(&this->org, sizeof(int), 1, fout);
	fwrite(&this->flags, sizeof(int), 1, fout);
	fwrite(this->buffer, sizeof(*this->buffer), this->pos, fout);

	return SYM_write(&this->sym, fout) + this->pos + 3 * sizeof(int);
}

int  ASM_load(ASM *this, const char *fn)
{
	FILE *fin;

	assert(fn&&*fn);

	fin = fopen(fn, "rb");

	assert(fin);

	fread(&this->pos, sizeof(int), 1, fin);
	fread(&this->org, sizeof(int), 1, fin);
	fread(&this->flags, sizeof(int), 1, fin);
	fread(this->buffer, sizeof(*this->buffer), this->pos, fin);

	SYM_read(&this->sym, fin);

	fclose(fin);

	return this->error;
}

void ASM_dispose(ASM *this)
{
	SYM_dispose(&this->sym);

	ASM_init(this);
}

// # ===========================================================================

void SYM_init(SYM_TABLE *this)
{
	this->syms = NULL;
	this->expr = NULL;
	this->occ  = NULL;
	this->cs = this->ce = 0;
}

int  SYM_find(SYM_TABLE *this, const char *id)
{
	int i;
	for(i = 0 ; i < this->cs ; i++)
	{
		if(strcmp(this->syms[i].name, id) == 0) return i;
	}

	i = this->cs;
	this->syms = realloc(this->syms, ++this->cs * sizeof(SYM));
	S_init(&this->syms[i]);
	strncpy(this->syms[i].name, id, SYM_NAME_LEN);
//	this->syms[i].name = strdup(id);

	return i;
}

void SYM_setValue(SYM_TABLE *this, const char *id, WORD v, int flags)
{
	int i = SYM_find(this, id);

	this->syms[i].val = v;
	this->syms[i].flags = flags;
}

void SYM_setExpression(SYM_TABLE *this, WORD o, const char *e)
{
	this->occ  = realloc(this->occ,  ++this->ce * sizeof(WORD));
	this->expr = realloc(this->expr,   this->ce * sizeof(char *));

	this->occ [this->ce - 1] = o;
	this->expr[this->ce - 1] = strdup(e);
}

void SYM_replaceSymbol(SYM_TABLE *this, char *dst, const char *src)
{
	int i;
	for(i = 0 ; i < this->cs ; i++)
	{
		if(strcmp(this->syms[i].name, src) == 0)
		{
			assert(!(this->syms[i].flags&SYM_F_EXTERN));

			sprintf(dst, "%hu", this->syms[i].val);
			return;
		}
	}

	strcpy(dst, src);
}

void SYM_replace(SYM_TABLE *this, WORD *data)
{
	TOKENIZER t;
	char buf[BUF_SIZE], *tmp;

	int i;
	for(i = 0 ; i < this->ce ; i++)
	{
		buf[0] = '\0';

		TOKENIZER_init(&t);
		TOKENIZER_set(&t, this->expr[i]);

		while((tmp = TOKENIZER_readToken(&t)) != NULL)
		{
			SYM_replaceSymbol(this, buf + strlen(buf), tmp);
		}

		assert(this->occ[i]>=0&&this->occ[i]<BUF_SIZE);

		//printf("EVAL: '%s' == ", buf);

		data[this->occ[i]] = (WORD) executeNumberEvaluator(buf);

		//printf("%d\n", data[this->occ[i]]);
	}
}

void SYM_setOffset(SYM_TABLE *this, int offset)
{
	int i;
	for(i = 0 ; i < this->cs ; i++)
	{
		if(!(this->syms[i].flags & SYM_F_ABS) && !(this->syms[i].flags & SYM_F_EXTERN))
		{
			this->syms[i].val += (WORD) offset;
		}
	}
}

int  SYM_resolveExternals(SYM_TABLE *this, ASM *assm, int c)
{
	int i, j, k;
	for(i = 0 ; i < this->cs ; i++)
	{
		if(this->syms[i].flags & SYM_F_EXTERN)
		{
			for(j = 0 ; j < c ; j++)
			{
				for(k = 0 ; k < assm[j].sym.cs ; k++)
				{
					if(!(assm[j].sym.syms[k].flags & SYM_F_EXTERN) && 
							strcmp(this->syms[i].name, assm[j].sym.syms[k].name) == 0)
					{
						this->syms[i].val = assm[j].sym.syms[k].val;
						this->syms[i].flags &= ~SYM_F_EXTERN;

						goto brk;
					}
				}
			}
	
			fprintf(stderr, "ERR: Couldn't resolve external label '%s'.\nAbort.\n", this->syms[i].name);
			exit(1);
	
brk: ;
		}
	}

	return 0;
}

long SYM_write(SYM_TABLE *this, FILE *fout)
{
	int i;
	long s = 0l;
	size_t l;

	fwrite(&this->cs, sizeof(int), 1, fout);
	fwrite(&this->ce, sizeof(int), 1, fout);

	s += 2 * sizeof(int);

	if(this->cs > 0)
	{
		fwrite(this->syms, sizeof(SYM), this->cs, fout);
	}

	s += this->cs * sizeof(SYM);

	if(this->ce > 0)
	{
		for(i = 0 ; i < this->ce ; i++)
		{
			l = strlen(this->expr[i]);
	
			fwrite(&l, sizeof(l), 1, fout);
			fwrite(this->expr[i], 1, l, fout);

			s += l + sizeof(l);
		}
	
		fwrite(this->occ, sizeof(WORD), this->ce, fout);

		s += this->ce * sizeof(WORD);
	}

	return s;
}

int SYM_read(SYM_TABLE *this, FILE *fin)
{
	int i;
	size_t l;

	fread(&this->cs, sizeof(int), 1, fin);
	fread(&this->ce, sizeof(int), 1, fin);

	if(this->cs > 0)
	{
		fread(this->syms, sizeof(SYM), this->cs, fin);
	}

	if(this->ce > 0)
	{
		this->expr = malloc(this->ce * sizeof(char *));
		this->occ  = malloc(this->ce * sizeof(WORD));

		for(i = 0 ; i < this->ce ; i++)
		{
			fread(&l, sizeof(l), 1, fin);
			this->expr[i] = malloc(l + 1);
			fread(this->expr[i], 1, l, fin);
			this->expr[i][l] = '\0';
		}

		fread(this->occ, sizeof(WORD), this->ce, fin);
	}

	return 0;
}

void SYM_dispose(SYM_TABLE *this)
{
	while(this->cs-- > 0)
	{
		S_dispose(&this->syms[this->cs]);
	}

	while(this->ce-- > 0)
	{
		free(this->expr[this->ce]);
	}

	free(this->syms);
	free(this->expr);
	free(this->occ);

	SYM_init(this);
}

// # ---------------------------------------------------------------------------

void S_init(SYM *this)
{
	this->name[0] = '\0';
	this->val  = 0;
}

void S_dispose(SYM *this)
{
	S_init(this);
}

// # ---------------------------------------------------------------------------

void INS_init(INS_TABLE *this)
{
	this->ins = NULL;
	this->c   = 0;
}

void INS_readInstructions(INS_TABLE *this, const char *fn)
{
	TOKENIZER t;
	FILE *fins;
	INST inst;
	char buf[BUF_SIZE], *tmp;
	long l;
	int d, i;

	assert(fn&&*fn);

	fins = fopen(fn, "r");

	assert(fins);

	fseek(fins, 0, SEEK_END);
	l = ftell(fins);
	fseek(fins, 0, SEEK_SET);

	assert(l<BUF_SIZE);

	fread(buf, 1, l, fins);
	buf[l] = '\0';

	fclose(fins);

	inst.name = NULL;
	inst.args[0] = inst.args[1] = INS_ARG_NONE;
	inst.op = -1;

	TOKENIZER_init(&t);
	TOKENIZER_set(&t, buf);

	INS_init(this);

	i = 0;
	while((tmp = TOKENIZER_readToken(&t)) != NULL)
	{
		if(sscanf(tmp, "%d", &d) == 1)
		{
			inst.op = d;
		}
		else if(inst.name == NULL)
		{
			inst.name = strdup(tmp);
			i = 0;
			inst.args[0] = inst.args[1] = INS_ARG_NONE;
		}
		else
		{
			switch(*tmp)
			{
				case '\n':
					INS_push(this, &inst);
					inst.name = NULL;
					break;
				case ',':
					i++;
					break;
				case 'c':
					inst.args[i] = INS_ARG_CONST;
					break;
				case 'r':
					inst.args[i] = INS_ARG_REG;
					break;
				case 'a':
					inst.args[i] = INS_ARG_MEM;
					break;
			}
		}

		free(tmp);

		assert(i<2);
	}

	if(inst.name != NULL)
	{
		INS_push(this, &inst);
	}

	TOKENIZER_dispose(&t);
}

void INS_push(INS_TABLE *this, INST *ins)
{
	this->ins = realloc(this->ins, ++this->c * sizeof(INST));
	memcpy(this->ins + this->c - 1, ins, sizeof(INST));
}

void INS_dispose(INS_TABLE *this)
{
	while(this->c-- > 0)
	{
		free(this->ins[this->c].name);
	}

	free(this->ins);

	INS_init(this);
}

// # ---------------------------------------------------------------------------

char *replaceCurPos(const char *src, const char *pos)
{
	TOKENIZER t;
	char buf[BUF_SIZE], *tmp;
	buf[0] = '\0';

	TOKENIZER_init(&t);
	TOKENIZER_set(&t, src);

	while((tmp = TOKENIZER_readToken(&t)) != NULL)
	{
		if(strcmp(tmp, "$") == 0)
		{
			strncat(buf, pos, BUF_SIZE);
		}
		else
		{
			strncat(buf, tmp, BUF_SIZE);
		}
	}

	return strdup(buf);
}

char *readMemory(const char *src, int *reg)
{
	TOKENIZER t;
	char buf[BUF_SIZE], *tmp;
	int valid = 0, rf = 0;
	buf[0] = '\0';

	TOKENIZER_init(&t);
	TOKENIZER_set(&t, src);

	*reg = -1;

	while((tmp = TOKENIZER_readToken(&t)) != NULL)
	{
		if(valid < 0) break;

		if(strcmp(tmp, "(") == 0)
		{
			if(valid) break;
			valid = 1;
		}
		else
		{
			if(!valid) break;

			if(strcmp(tmp, ")") == 0)
			{
				valid = -1;
			}
			else if(*tmp == 'r')
			{
				if(rf) break;

				sscanf(tmp, "r%d", reg);
				strncat(buf, "0", BUF_SIZE);
				rf = 1;
			}
			else
			{
				strncat(buf, tmp, BUF_SIZE);
			}
		}
	}

	TOKENIZER_dispose(&t);

	if(!rf || valid != -1) return NULL;

	return strdup(buf);
}

