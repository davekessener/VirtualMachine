#include "NBT.h"

#define MXT_RWORD(v) ((((v) & 0xff) << 8) | (((v) >> 8) & 0xff))
#define MXT_RDWORD(v) (MXT_RWORD(((v) >> 16) & 0xffff) | (MXT_RWORD((v) & 0xffff) << 16))
#define MXT_RQWORD(v) (MXT_RDWORD(((v) >> 32) & 0xffffffff) | (MXT_RDWORD((v) & 0xffffffff) << 32))

//#define MXT_RWORD(v) (v)
//#define MXT_RDWORD(v) (v)
//#define MXT_RQWORD(v) (v)

NBT *read_named_tag(FILE *, BYTE);
void *read_unnamed_tag(FILE *, BYTE);

char *read_name(FILE *f)
{
	WORD l;
	fread(&l, 1, sizeof(WORD), f);
	l = MXT_RWORD(l);
	char *b = malloc(l + 1);
	if(l) fread(b, l, sizeof(char), f);
	b[l] = '\0';
	fprintf(stderr, "Read string length %d: '%s'\n", l, b);
	return b;
}

void *read_BYTE(FILE *f)
{
	BYTE *data = malloc(sizeof(BYTE));
	*data = getc(f);
	return data;
}

void *read_SHORT(FILE *f)
{
	WORD *data = malloc(sizeof(WORD));
	fread(data, 1, sizeof(WORD), f);
	*data = MXT_RWORD(*data);
	return data;
}

void *read_INT(FILE *f)
{
	DWORD *data = malloc(sizeof(DWORD));
	fread(data, 1, sizeof(DWORD), f);
	*data = MXT_RDWORD(*data);
	return data;
}

void *read_LONG(FILE *f)
{
	QWORD *data = malloc(sizeof(QWORD));
	fread(data, 1, sizeof(QWORD), f);
	*data = MXT_RQWORD(*data);
	return data;
}

void *read_FLOAT(FILE *f)
{
	float *data = malloc(sizeof(float));
	fread(data, 1, sizeof(float), f);
	return data;
}

void *read_DOUBLE(FILE *f)
{
	double *data = malloc(sizeof(double));
	fread(data, 1, sizeof(double), f);
	return data;
}

void *read_BYTE_ARRAY(FILE *f)
{
	DWORD l;
	fread(&l, 1, sizeof(DWORD), f);
	l = MXT_RDWORD(l);
	NBT_Array *d = malloc(sizeof(NBT_Array));
	d->c = d->i = l;
	d->data = malloc(l);
	fread(d->data, l, sizeof(BYTE), f);
	return d;
}

void *read_STRING(FILE *f)
{
	return read_name(f);
}

void *read_LIST(FILE *f)
{
	NBT_List *list = malloc(sizeof(NBT_List));
	list->type = getc(f);
	fread(&list->c, 1, sizeof(DWORD), f);
	list->c = MXT_RDWORD(list->c);
	list->data = malloc(list->c * sizeof(void *));

	for(list->i = 0 ; list->i < list->c ; ++list->i)
	{
		list->data[list->i] = read_unnamed_tag(f, list->type);
	}

	return list;
}

void insert(NBT_Compound *c, NBT *tag)
{
	if(c->i == c->c)
	{
		c->tags = realloc(c->tags, sizeof(NBT *) * (c->c = 2 * (c->c + 1)));
	}

	c->tags[c->i++] = tag;
}

void *read_COMPOUND(FILE *f)
{
	NBT_Compound *tag = malloc(sizeof(NBT_Compound));
	BYTE id;

	tag->c = tag->i = 0;
	tag->tags = NULL;

	while((id = getc(f)))
	{
		insert(tag, read_named_tag(f, id));
	}

	return tag;
}

void *read_INT_ARRAY(FILE *f)
{
	DWORD l;
	fread(&l, 1, sizeof(DWORD), f);
	l = MXT_RDWORD(l);
	NBT_Array *d = malloc(sizeof(NBT_Array));
	d->c = d->i = l;
	d->data = malloc(l * sizeof(DWORD));
	fread(d->data, l, sizeof(DWORD), f);

	int i;
	DWORD *p = (DWORD *) d->data;
	for(i = 0 ; i < l ; ++i)
	{
		p[i] = MXT_RDWORD(p[i]);
	}

	return d;
}

NBT *nbt_read(FILE *f)
{
	return read_named_tag(f, getc(f));
}

NBT *read_named_tag(FILE *f, BYTE t)
{
	NBT *tag = malloc(sizeof(NBT));
	tag->name = read_name(f);
	tag->type = t;
	tag->data = read_unnamed_tag(f, t);
	return tag;
}

void *read_unnamed_tag(FILE *f, BYTE t)
{
	switch(t)
	{
		case NBT_BYTE: return read_BYTE(f);
		case NBT_SHORT: return read_SHORT(f);
		case NBT_INT: return read_INT(f);
		case NBT_LONG: return read_LONG(f);
		case NBT_FLOAT: return read_FLOAT(f);
		case NBT_DOUBLE: return read_DOUBLE(f);
		case NBT_BYTE_ARRAY: return read_BYTE_ARRAY(f);
		case NBT_STRING: return read_STRING(f);
		case NBT_LIST: return read_LIST(f);
		case NBT_COMPOUND: return read_COMPOUND(f);
		case NBT_INT_ARRAY: return read_INT_ARRAY(f);
		default: assert(!"Can't read this tag-type.");
	}
}

// # ===========================================================================

void write_named_tag(FILE *, const NBT *);
void write_unnamed_tag(FILE *, const void *, BYTE);

void write_BYTE(FILE *f, const void *d)
{
	fwrite(d, 1, sizeof(BYTE), f);
}

void write_SHORT(FILE *f, const void *d)
{
	WORD v = MXT_RWORD(*(const WORD *)d);
	fwrite(&v, 1, sizeof(WORD), f);
}

void write_INT(FILE *f, const void *d)
{
	DWORD v = MXT_RDWORD(*(const DWORD *)d);
	fwrite(&v, 1, sizeof(DWORD), f);
}

void write_LONG(FILE *f, const void *d)
{
	QWORD v = MXT_RQWORD(*(const QWORD *)d);
	fwrite(&v, 1, sizeof(QWORD), f);
}

void write_FLOAT(FILE *f, const void *d)
{
	fwrite(d, 1, sizeof(float), f);
}

void write_DOUBLE(FILE *f, const void *d)
{
	fwrite(d, 1, sizeof(double), f);
}

void write_BYTE_ARRAY(FILE *f, const void *d)
{
	const NBT_Array *a = (const NBT_Array *) d;
	DWORD v = MXT_RDWORD(a->i);
	fwrite(&v, 1, sizeof(DWORD), f);
	if(a->i) fwrite(a->data, a->i, sizeof(BYTE), f);
}

void write_STRING(FILE *f, const void *d)
{
	WORD nl = strlen((const char *) d);
	WORD v = MXT_RWORD(nl);
	fwrite(&v, 1, sizeof(WORD), f);
	if(nl) fwrite(d, nl, sizeof(char), f);
}

void write_LIST(FILE *f, const void *d)
{
	const NBT_List *list = (const NBT_List *) d;
	putc(list->type, f);
	DWORD v = MXT_RDWORD(list->i);
	fwrite(&v, 1, sizeof(DWORD), f);

	int i;
	for(i = 0 ; i < list->i ; ++i)
	{
		write_unnamed_tag(f, list->data[i], list->type);
	}
}

void write_COMPOUND(FILE *f, const void *d)
{
	const NBT_Compound *nbt = (const NBT_Compound *) d;

	int i;
	for(i = 0 ; i < nbt->i ; ++i)
	{
		write_named_tag(f, nbt->tags[i]);
	}

	putc('\0', f);
}

void write_INT_ARRAY(FILE *f, const void *d)
{
	const NBT_Array *a = (const NBT_Array *) d;
	DWORD v = MXT_RDWORD(a->i);
	fwrite(&v, 1, sizeof(DWORD), f);
	
	int i;
	for(i = 0 ; i < a->i ; ++i)
	{
		DWORD vv = MXT_RDWORD(((const DWORD *) a->data)[i]);
		fwrite(&vv, 1, sizeof(DWORD), f);
	}
}

void write_unnamed_tag(FILE *f, const void *data, BYTE type)
{
	switch(type)
	{
		case NBT_BYTE: write_BYTE(f, data); break;
		case NBT_SHORT: write_SHORT(f, data); break;
		case NBT_INT: write_INT(f, data); break;
		case NBT_LONG: write_LONG(f, data); break;
		case NBT_FLOAT: write_FLOAT(f, data); break;
		case NBT_DOUBLE: write_DOUBLE(f, data); break;
		case NBT_BYTE_ARRAY: write_BYTE_ARRAY(f, data); break;
		case NBT_STRING: write_STRING(f, data); break;
		case NBT_LIST: write_LIST(f, data); break;
		case NBT_COMPOUND: write_COMPOUND(f, data); break;
		case NBT_INT_ARRAY: write_INT_ARRAY(f, data); break;
		default: assert(!"Can't write this tag-type.");
	}
}

void write_named_tag(FILE *f, const NBT *nbt)
{
	putc(nbt->type, f);
	write_STRING(f, nbt->name);
	write_unnamed_tag(f, nbt->data, nbt->type);
}

void nbt_write(const NBT *nbt, FILE *f)
{
	write_named_tag(f, nbt);
}

// # =========================================================================== 

void delete_unnamed_tag(void *, BYTE);
void delete_named_tag(NBT *);

void delete_LIST(NBT_List *list)
{
	int i;
	for(i = 0 ; i < list->i ; ++i)
	{
		delete_unnamed_tag(list->data[i], list->type);
	}

	free(list->data);
}

void delete_COMPOUND(NBT_Compound *nbt)
{
	int i;
	for(i = 0 ; i < nbt->i ; ++i)
	{
		delete_named_tag(nbt->tags[i]);
	}

	free(nbt->tags);
}

void delete_unnamed_tag(void *d, BYTE t)
{
	switch(t)
	{
		case NBT_BYTE:
		case NBT_SHORT:
		case NBT_INT:
		case NBT_LONG:
		case NBT_FLOAT:
		case NBT_DOUBLE:
		case NBT_STRING:
			break;
		case NBT_BYTE_ARRAY:
		case NBT_INT_ARRAY:
			free(((NBT_Array *) d)->data);
			break;
		case NBT_LIST:
			delete_LIST((NBT_List *) d);
			break;
		case NBT_COMPOUND:
			delete_COMPOUND((NBT_Compound *) d);
			break;
		default: assert(!"Unknown tag type!");
	}

	free(d);
}

void delete_named_tag(NBT *nbt)
{
	free((char *) nbt->name);
	delete_unnamed_tag(nbt->data, nbt->type);
	free(nbt);
}

void nbt_delete(NBT *nbt)
{
	delete_named_tag(nbt);
}

