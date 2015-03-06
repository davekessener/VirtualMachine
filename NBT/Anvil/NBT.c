#include "NBT.h"

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
	return data;
}

void *read_INT(FILE *f)
{
	DWORD *data = malloc(sizeof(DWORD));
	fread(data, 1, sizeof(DWORD), f);
	return data;
}

void *read_LONG(FILE *f)
{
	QWORD *data = malloc(sizeof(QWORD));
	fread(data, 1, sizeof(QWORD), f);
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
		c->data = realloc(c->data, c->c = 2 * (c->c + 1));
	}

	c->data[c->i++] = tag;
}

void *read_COMPOUND(FILE *f)
{
	NBT_Compound *tag = malloc(sizeof(NBT_Compound));
	BYTE id;

	tag->c = tag->i = 0;
	tag->data = NULL;

	while(id = getc(f))
	{
		insert(tag, read_named_tag(f, id));
	}
}

void *read_INT_ARRAY(FILE *f)
{
	DWORD l;
	fread(&l, 1, sizeof(DWORD), f);
	NBT_Array *d = malloc(sizeof(NBT_Array));
	d->c = d->i = l;
	d->data = malloc(l * sizeof(DWORD));
	fread(d->data, l, sizeof(DWORD), f);
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
		default: assert(false);
	}
}

void nbt_write(const NBT *nbt, FILE *f)
{
}

