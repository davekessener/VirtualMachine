#define PARAMETER_C
#include "parameter.h"
#undef PARAMETER_C

void   PTABLE_init(PTABLE *this)
{
	this->types    = NULL;
	this->params   = NULL;
	this->tc       = 0;
	this->pc       = 0;
	this->status   = STATUS_OK;
	this->name     = NULL;
	this->fullName = NULL;
}

void   PTABLE_addParameter(PTABLE *this, const char *id, const char *fid, int t)
{
	this->types = realloc(this->types, ++this->tc * sizeof(PTYPE));
	PTYPE_init(this->types + this->tc - 1, id, fid, t);
}

void   PTABLE_read(PTABLE *this, int c, char **v)
{
	int resetFlag = 0, curP = 0;
	int i, j, p;
	char *tmp, *name, *body;

	this->fullName = this->name = strdup(v[0]);

	tmp = this->name;
	while(*tmp) { if(*tmp == '/') this->name = tmp + 1; tmp++; }

	this->params = malloc(++this->pc * sizeof(PARA));
	PARA_init(this->params, NULL);

	for(i = 1 ; i < c ; i++)
	{
		if(v[i][0] == '-')
		{
			if(v[i][1] == '-')
			{
				this->status |= STATUS_UNKNOWN;
				name = strdup(v[i] + 2);
				body = NULL;

				for(p = 0 ; name[p] ; p++) { if(name[p] == '=') { name[p] = '\0'; body = name + p + 1; } }

				for(j = 0 ; j < this->tc ; j++)
				{
					if(this->types[j].fullName == NULL)
					{
						continue;
					}

					if(strcmp(this->types[j].fullName, name) == 0)
					{
						p = PTABLE_getIndex(this, this->types[j].fullName);
						if(p > 0)
						{
							this->params[p].status |= STATUS_DUPLICATE;
						}
						else
						{
							p = this->pc++;
							this->params = realloc(this->params, this->pc * sizeof(PARA));
							PARA_init(this->params + p, this->types[j].fullName);
						}

						if(body) PARA_addValue(this->params + p, body);

						curP = 0;
						this->status &= ~STATUS_UNKNOWN;
						break;
					}
				}

				free(name);
			}
			else
			{
				this->status |= STATUS_UNKNOWN;

				for(j = 0 ; j < this->tc ; j++)
				{
					if(this->types[j].shortName != NULL && strcmp(this->types[j].shortName, v[i] + 1) == 0)
					{
						p = PTABLE_getIndex(this, this->types[j].shortName);

						if(p > 0)
						{
							this->params[p].status |= STATUS_DUPLICATE;
						}
						else
						{
							p = this->pc++;
							this->params = realloc(this->params, this->pc * sizeof(PARA));
							PARA_init(this->params + p, this->types[j].shortName);
						}

						switch(this->types[j].argType)
						{
							case PARAM_NONE:
								curP = 0;
								resetFlag = 0;
								break;
							case PARAM_ONE:
								curP = p;
								resetFlag = 1;
								break;
							case PARAM_MANY:
								curP = p;
								resetFlag = 0;
								break;
						}

						this->status &= ~STATUS_UNKNOWN;

						break;
					}
				}
			}
		}
		else
		{
			PARA_addValue(this->params + curP, v[i]);
			if(resetFlag) { resetFlag = 0; curP = 0; }
		}
	}
}

int    PTABLE_hasArgument(PTABLE *this, const char *id)
{
	return PTABLE_getIndex(this, id) >= 0;
}

char  *PTABLE_getValue(PTABLE *this, const char *id)
{
	int i = PTABLE_getIndex(this, id);

	if(i < 0) return NULL;

	return this->params[i].vals[0];
}

char **PTABLE_getValues(PTABLE *this, const char *id)
{
	int i = PTABLE_getIndex(this, id);

	if(i < 0) return NULL;

	return this->params[i].vals;
}

void   PTABLE_dispose(PTABLE *this)
{
	int i;
	for(i = 0 ; i < this->tc ; i++)
	{
		PTYPE_dispose(&this->types[i]);
	}

	for(i = 0 ; i < this->pc ; i++)
	{
		PARA_dispose(&this->params[i]);
	}

	free(this->types);
	free(this->params);

	PTABLE_init(this);
}

// # ---------------------------------------------------------------------------

int    PTABLE_getIndex(PTABLE *this, const char *id)
{
	const char *sn = NULL, *fn = NULL;
	int i;

	if(id == NULL) return 0;

	for(i = 0 ; i < this->tc ; i++)
	{
		if(this->types[i].shortName != NULL && strcmp(this->types[i].shortName, id) == 0)
		{
			sn = id;
			fn = this->types[i].fullName;
			break;
		}
		else if(this->types[i].fullName != NULL && strcmp(this->types[i].fullName, id) == 0)
		{
			sn = this->types[i].shortName;
			fn = id;
			break;
		}
	}

	if(sn == NULL && fn == NULL) return -1;

	for(i = 1 ; i < this->pc ; i++)
	{
		if((sn != NULL && strcmp(this->params[i].id, sn) == 0) ||
			(fn != NULL && strcmp(this->params[i].id, fn) == 0))
		{
			return i;
		}
	}

	return -1;
}

// # ===========================================================================

void PTYPE_init(PTYPE *this, const char *id, const char *fid, int t)
{
	this->shortName = id == NULL ? NULL : strdup(id);
	this->fullName = fid == NULL ? NULL : strdup(fid);
	this->argType = t;
}

void PTYPE_dispose(PTYPE *this)
{
	free(this->shortName);
	free(this->fullName);

	this->shortName = this->fullName = NULL;
	this->argType = PARAM_NONE;
}

// # ---------------------------------------------------------------------------

void PARA_init(PARA *this, const char *id)
{
	this->id      = id == NULL ? NULL : strdup(id);
	this->vals    = malloc(sizeof(char *));
	this->vals[0] = NULL;
	this->status  = STATUS_OK;
}

int  PARA_getSize(PARA *this)
{
	int i;
	if(this->vals == NULL) return 0;
	
	i = 0;
	while(this->vals[i] != NULL) i++;

	return i;
}

void PARA_addValue(PARA *this, const char *value)
{
	int l = PARA_getSize(this) + 2;
	this->vals = realloc(this->vals, l * sizeof(char *));
	this->vals[l - 2] = strdup(value);
	this->vals[l - 1] = NULL;
}

void PARA_dispose(PARA *this)
{
	int i, l = PARA_getSize(this);
	for(i = 0 ; i < l ; i++)
	{
		free(this->vals[i]);
	}

	free(this->id);
	free(this->vals);

	this->id   = NULL;
	this->vals = NULL;
}

