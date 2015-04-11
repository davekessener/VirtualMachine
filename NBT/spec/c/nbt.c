#include <string.h>
#include "nbt.h"

typedef struct
{
	union
	{
		const BYTE *array;
		FILE *file;
	} content;
	BYTE type;
} wrapper_t;

enum
{
	FILE_WRAPPER,
	STREAM_WRAPPER
};

void *read_payload(BYTE, wrapper_t *);
void write_payload(FILE *, BYTE, const void *);
void delete_payload(BYTE, void *);

int error = ERR_NBT_NONE;

const char *nbt_error_msg(void)
{
	return NULL;
}

void nbt_clear_error(void)
{
	error = ERR_NBT_NONE;
}

int nbt_error(void)
{
	return error;
}

/* ======================================================================== */

void *wrapper_straight(wrapper_t *in, size_t v, void *d)
{
	BYTE *buf = d ? (BYTE *) d : malloc(v);

	if(in) switch(in->type)
	{
		case FILE_WRAPPER:
			fread(buf, 1, v, in->content.file);
			break;
		case STREAM_WRAPPER:
			memcpy(buf, in->content.array, v);
			in->content.array += v;
			break;
	}
	else
	{
		memset(buf, 0, v);
	}

	return buf;
}

void *wrapper_read(wrapper_t *in, size_t v, void *d)
{
	BYTE *buf = d ? (BYTE *) d : malloc(v);
	BYTE *e = buf + v, *b = buf;

	if(v == 1) return wrapper_straight(in, v, buf);

	while(b != e)
	{
		BYTE t = 0;

		if(in) switch(in->type)
		{
			case FILE_WRAPPER:
				t = getc(in->content.file);
				break;
			case STREAM_WRAPPER:
				t = *in->content.array++;
				break;
		}

		*--e = t;
	}

	return buf;
}

char *read_name(wrapper_t *in)
{
	char *name = NULL;

	if(in)
	{
		WORD l;

		wrapper_read(in, sizeof(l), &l);

		name = malloc(l);

		wrapper_straight(in, l, name);
	}

	return name;
}

void reverse(BYTE *t, size_t v)
{
	uint i;
	for(i = 0 ; i < v / 2 ; ++i)
	{
		BYTE tt = t[i];
		t[i] = t[v - i - 1];
		t[v - i - 1] = tt;
	}
}

NBT_Array *read_array(wrapper_t *in, size_t v)
{
	NBT_Array *tag = malloc(sizeof(NBT_Array));
	DWORD l;

	wrapper_read(in, sizeof(l), &l);

	tag->data = NULL;
	tag->c = tag->i = l;
	tag->type = v;

	if(l)
	{
		wrapper_straight(in, l * v, tag->data = malloc(l * v));

		if(v > 1)
		{
			BYTE *t = tag->data;
			int i = l;

			while(i--)
			{
				reverse(t, v);
				t += v;
			}
		}
	}

	return tag;
}

NBT_List *read_list(wrapper_t *in)
{
	NBT_List *tag = malloc(sizeof(NBT_List));

	BYTE id;
	DWORD l;

	wrapper_read(in, sizeof(id), &id);
	wrapper_read(in, sizeof(l), &l);

	tag->id = id;
	tag->i = tag->c = l;
	tag->payloads = NULL;

	if(l)
	{
		uint i;

		tag->payloads = malloc(l * sizeof(void *));

		for(i = 0 ; i < l ; ++i)
		{
			tag->payloads[i] = read_payload(id, in);
		}
	}

	return tag;
}

uint get_tag(const NBT_Compound *nbt, const char *name)
{
	uint i;
	for(i = 0 ; i < nbt->i ; ++i)
	{
		if(strcmp(name, nbt->tags[i]->name) == 0) return i + 1;
	}

	return 0;
}

void *resize(void *data, uint *i, uint *c, size_t v)
{
	if(*i == *c)
	{
		data = realloc(data, v * (*c = *c * 2 + 1));
	}

	return data;
}

void push_tag(NBT_Compound *nbt, NBTTag *tag)
{
	uint i = get_tag(nbt, tag->name);

	if(!i)
	{
		nbt->tags = resize(nbt->tags, &nbt->i, &nbt->c, sizeof(*nbt->tags));
		i = nbt->i++;
	}

	nbt->tags[i] = tag;
}

NBTTag *read_tag(wrapper_t *in)
{
	NBTTag *tag = NULL;
	BYTE id;

	wrapper_read(in, sizeof(id), &id);

	if(!id) return NULL;

	tag = malloc(sizeof(NBTTag));

	tag->id = id;
	tag->name = read_name(in);
	tag->tag = read_payload(tag->id, in);

	return tag;
}

NBT_Compound *read_compound(wrapper_t *in)
{
	NBT_Compound *tag = malloc(sizeof(NBT_Compound));

	tag->i = tag->c = 0;
	tag->tags = NULL;

	while(1)
	{
		NBTTag *nbt = read_tag(in);

		if(!nbt) break;

		push_tag(tag, nbt);
	}

	return tag;
}

void *read_payload(BYTE id, wrapper_t *in)
{
	switch(id)
	{
		case TAG_BYTE:       return wrapper_read(in, sizeof(BYTE), NULL);
		case TAG_SHORT:      return wrapper_read(in, sizeof(WORD), NULL);
		case TAG_INT:        return wrapper_read(in, sizeof(DWORD), NULL);
		case TAG_LONG:       return wrapper_read(in, sizeof(QWORD), NULL);
		case TAG_FLOAT:      return wrapper_read(in, sizeof(float), NULL);
		case TAG_DOUBLE:     return wrapper_read(in, sizeof(double), NULL);
		case TAG_STRING:     return read_name(in);
		case TAG_BYTE_ARRAY: return read_array(in, sizeof(BYTE));
		case TAG_COMPOUND:   return read_compound(in);
		case TAG_LIST:       return read_list(in);
		case TAG_INT_ARRAY:  return read_array(in, sizeof(DWORD));
	}

	return NULL;
}

NBTTag *read_nbt_from_file(FILE *f)
{
	wrapper_t w;

	w.content.file = f;
	w.type = FILE_WRAPPER;

	return read_tag(&w);
}

NBTTag *read_nbt(const BYTE *s)
{
	wrapper_t w;

	w.content.array = s;
	w.type = STREAM_WRAPPER;

	return read_tag(&w);
}

NBTTag *nbt_create(BYTE id, const char *name, void *pl)
{
	NBTTag *tag = malloc(sizeof(NBTTag));

	tag->id = id;
	tag->name = name ? strdup(name) : NULL;
	tag->tag = pl ? pl : read_payload(id, NULL);

	return tag;
}

void write_scrambled(FILE *f, size_t v, const void *d)
{
	const BYTE *b = (const BYTE *) d, *e = b + v;

	while(b != e)
	{
		putc(*--e, f);
	}
}

void write_BYTE(FILE *f, BYTE id)
{
	putc(id, f);
}

void write_string(FILE *f, const char *s)
{
	WORD l = strlen(s);

	write_scrambled(f, sizeof(l), &l);
	fwrite(s, sizeof(char), l, f);
}

void write_array(FILE *f, const NBT_Array *a)
{
	const BYTE *b = a->data;
	uint i;

	write_scrambled(f, sizeof(a->i), &a->i);

	if(a->type > 1) for(i = 0 ; i < a->i ; ++i)
	{
		write_scrambled(f, a->type, b + i * a->type);
	}
	else
	{
		fwrite(a->data, 1, a->i, f);
	}
}

void write_list(FILE *f, const NBT_List *list)
{
	write_BYTE(f, list->id);

	write_scrambled(f, sizeof(list->i), &list->i);

	uint i;
	for(i = 0 ; i < list->i ; ++i)
	{
		write_payload(f, list->id, list->payloads[i]);
	}
}

void write_tag(FILE *f, const NBTTag *tag)
{
	write_BYTE(f, tag->id);
	write_string(f, tag->name);
	write_payload(f, tag->id, tag->tag);
}

void write_compound(FILE *f, const NBT_Compound *nbt)
{
	uint i;
	for(i = 0 ; i < nbt->i ; ++i)
	{
		write_tag(f, nbt->tags[i]);
	}

	write_BYTE(f, TAG_NONE);
}

void write_payload(FILE *f, BYTE id, const void *pl)
{
	switch(id)
	{
		case TAG_BYTE: write_scrambled(f, sizeof(BYTE), pl); break;
		case TAG_SHORT: write_scrambled(f, sizeof(WORD), pl); break;
		case TAG_INT: write_scrambled(f, sizeof(DWORD), pl); break;
		case TAG_LONG: write_scrambled(f, sizeof(QWORD), pl); break;
		case TAG_FLOAT: write_scrambled(f, sizeof(float), pl); break;
		case TAG_DOUBLE: write_scrambled(f, sizeof(double), pl); break;
		case TAG_BYTE_ARRAY: write_array(f, (NBT_Array *) pl); break;
		case TAG_STRING: write_string(f, (const char *) pl); break;
		case TAG_LIST: write_list(f, (NBT_List *) pl); break;
		case TAG_COMPOUND: write_compound(f, (NBT_Compound *) pl); break;
		case TAG_INT_ARRAY: write_array(f, (NBT_Array *) pl); break;
	}
}

void write_nbt_to_file(FILE *f, const NBTTag *tag)
{
	write_tag(f, tag);
}

// ===========================================================================

void delete_array(NBT_Array *a)
{
	free(a->data);
	free(a);
}

void delete_list(NBT_List *list)
{
	uint i;
	for(i = 0 ; i < list->i ; ++i)
	{
		delete_payload(list->id, list->payloads[i]);
	}

	free(list->payloads);
	free(list);
}

void delete_compound(NBT_Compound *nbt)
{
	uint i;
	for(i = 0 ; i < nbt->i ; ++i)
	{
		nbt_delete(nbt->tags[i]);
	}

	free(nbt->tags);
	free(nbt);
}

void delete_payload(BYTE id, void *pl)
{
	switch(id)
	{
		case TAG_BYTE:
		case TAG_SHORT:
		case TAG_INT:
		case TAG_LONG:
		case TAG_FLOAT:
		case TAG_DOUBLE:
		case TAG_STRING:
			free(pl);
			break;
		case TAG_BYTE_ARRAY:
		case TAG_INT_ARRAY:
			delete_array((NBT_Array *) pl);
			break;
		case TAG_LIST:
			delete_list((NBT_List *) pl);
			break;
		case TAG_COMPOUND:
			delete_compound((NBT_Compound *) pl);
			break;
	}
}

void nbt_delete(NBTTag *tag)
{
	free(tag->name);
	delete_payload(tag->id, tag->tag);
	free(tag);
}

BYTE nbt_get_id(NBTTag *tag)
{
	return tag->id;
}

const char *nbt_get_name(NBTTag *tag)
{
	return tag->name;
}

// ===========================================================================

void nbt_compound_set_byte(NBTTag *tag, const char *name, BYTE v)
{
	NBTTag *nbt = nbt_create(TAG_BYTE, name, NULL);
	BYTE *pv = malloc(sizeof(BYTE));

	*pv = v;

	nbt->tag = pv;

	push_tag((NBT_Compound *) tag->tag, nbt);
}

BYTE nbt_compound_get_byte(const NBTTag *tag, const char *name)
{
	const NBT_Compound *nbt = (const NBT_Compound *) tag->tag;
	uint i = get_tag(nbt, name);

	return *(const BYTE *) nbt->tags[i]->tag;
}

void nbt_compound_set_short(NBTTag *tag, const char *name, WORD v)
{
	NBTTag *nbt = nbt_create(TAG_SHORT, name, NULL);
	WORD *pv = malloc(sizeof(WORD));

	*pv = v;

	nbt->tag = pv;

	push_tag((NBT_Compound *) tag->tag, nbt);
}

WORD nbt_compound_get_short(const NBTTag *tag, const char *name)
{
	const NBT_Compound *nbt = (const NBT_Compound *) tag->tag;
	uint i = get_tag(nbt, name);

	return *(const WORD *) nbt->tags[i]->tag;
}

void nbt_compound_set_int(NBTTag *tag, const char *name, DWORD v)
{
	NBTTag *nbt = nbt_create(TAG_INT, name, NULL);
	DWORD *pv = malloc(sizeof(DWORD));

	*pv = v;

	nbt->tag = pv;

	push_tag((NBT_Compound *) tag->tag, nbt);
}

DWORD nbt_compound_get_int(const NBTTag *tag, const char *name)
{
	const NBT_Compound *nbt = (const NBT_Compound *) tag->tag;
	uint i = get_tag(nbt, name);

	return *(const DWORD *) nbt->tags[i]->tag;
}

void nbt_compound_set_long(NBTTag *tag, const char *name, QWORD v)
{
	NBTTag *nbt = nbt_create(TAG_LONG, name, NULL);
	QWORD *pv = malloc(sizeof(QWORD));

	*pv = v;

	nbt->tag = pv;

	push_tag((NBT_Compound *) tag->tag, nbt);
}

QWORD nbt_compound_get_long(const NBTTag *tag, const char *name)
{
	const NBT_Compound *nbt = (const NBT_Compound *) tag->tag;
	uint i = get_tag(nbt, name);

	return *(const QWORD *) nbt->tags[i]->tag;
}

void nbt_compound_set_float(NBTTag *tag, const char *name, float v)
{
	NBTTag *nbt = nbt_create(TAG_FLOAT, name, NULL);
	float *pv = malloc(sizeof(float));

	*pv = v;

	nbt->tag = pv;

	push_tag((NBT_Compound *) tag->tag, nbt);
}

float nbt_compound_get_float(const NBTTag *tag, const char *name)
{
	const NBT_Compound *nbt = (const NBT_Compound *) tag->tag;
	uint i = get_tag(nbt, name);

	return *(const float *) nbt->tags[i]->tag;
}

void nbt_compound_set_double(NBTTag *tag, const char *name, double v)
{
	NBTTag *nbt = nbt_create(TAG_DOUBLE, name, NULL);
	double *pv = malloc(sizeof(double));

	*pv = v;

	nbt->tag = pv;

	push_tag((NBT_Compound *) tag->tag, nbt);
}

double nbt_compound_get_double(const NBTTag *tag, const char *name)
{
	const NBT_Compound *nbt = (const NBT_Compound *) tag->tag;
	uint i = get_tag(nbt, name);

	return *(const double *) nbt->tags[i]->tag;
}

void nbt_compound_set_string(NBTTag *tag, const char *name, const char *s)
{
	NBTTag *nbt = nbt_create(TAG_STRING, name, NULL);
	nbt->tag = strdup(s);
	push_tag((NBT_Compound *) tag->tag, nbt);
}

const char *nbt_compound_get_string(const NBTTag *tag, const char *name)
{
	const NBT_Compound *nbt = (const NBT_Compound *) tag->tag;
	uint i = get_tag(nbt, name);

	return (const char *) nbt->tags[i]->tag;
}

void nbt_compound_set_tag(NBTTag *tag, const char *name, NBTTag *nbt)
{
	if(name)
	{
		free(nbt->name);
		nbt->name = strdup(name);
	}

	push_tag((NBT_Compound *) tag->tag, nbt);
}

NBTTag *nbt_compound_get_tag(NBTTag *tag, const char *name)
{
	uint i = get_tag((NBT_Compound *) tag->tag, name);

	return ((NBT_Compound *) tag->tag)->tags[i];
}

void nbt_compound_set_compoundtag(NBTTag *tag, const char *name, NBTTag *nbt)
{
	nbt_compound_set_tag(tag, name, nbt);
}

NBTTag *nbt_compound_get_compoundtag(NBTTag *tag, const char *name)
{
	return nbt_compound_get_tag(tag, name);
}

void nbt_compound_set_list(NBTTag *tag, const char *name, NBTTag *nbt)
{
	nbt_compound_set_tag(tag, name, nbt);
}

NBTTag *nbt_compound_get_list(NBTTag *tag, const char *name)
{
	return nbt_compound_get_tag(tag, name);
}

void nbt_compound_set_byte_array(NBTTag *tag, const char *name, const BYTE *i1, const BYTE *i2)
{
	NBTTag *nbt = nbt_create(TAG_BYTE_ARRAY, name, NULL);
	NBT_Array *a = malloc(sizeof(NBT_Array));

	a->type = sizeof(BYTE);
	a->i = a->c = i2 - i1;
	a->data = malloc(a->c);

	memcpy(a->data, i1, a->i * a->type);

	nbt->tag = a;

	push_tag((NBT_Compound *) tag->tag, nbt);
}

const BYTE *nbt_compound_get_byte_array(const NBTTag *tag, const char *name, DWORD *l)
{
	const NBT_Array *a = (const NBT_Array *) tag->tag;
	
	if(l) *l = a->i;

	return (const BYTE *) a->data;
}

void nbt_compound_set_int_array(NBTTag *tag, const char *name, const DWORD *i1, const DWORD *i2)
{
	NBTTag *nbt = nbt_create(TAG_INT_ARRAY, name, NULL);
	NBT_Array *a = malloc(sizeof(NBT_Array));

	a->type = sizeof(DWORD);
	a->i = a->c = i2 - i1;
	a->data = malloc(a->c);

	memcpy(a->data, i1, a->i * a->type);

	nbt->tag = a;

	push_tag((NBT_Compound *) tag->tag, nbt);
}

const DWORD *nbt_compound_get_int_array(const NBTTag *tag, const char *name, DWORD *l)
{
	const NBT_Array *a = (const NBT_Array *) tag->tag;
	
	if(l) *l = a->i;

	return (const DWORD *) a->data;
}

// # ==========================================================================

void nbt_list_add_tag(NBTTag *tag, NBTTag *nbt)
{
	NBT_List *list = (NBT_List *) tag->tag;

	list->payloads = resize(list->payloads, &list->i, &list->c, sizeof(*list->payloads));

	list->payloads[list->i++] = nbt->tag;
}

void *nbt_list_begin(NBTTag *tag)
{
	return ((NBT_List *) tag->tag)->payloads;
}

void *nbt_list_end(NBTTag *tag)
{
	return nbt_list_begin(tag) + nbt_list_size(tag);
}

size_t nbt_list_size(const NBTTag *tag)
{
	return ((const NBT_List *) tag->tag)->i;
}

BYTE nbt_list_type(const NBTTag *tag)
{
	return ((const NBT_List *) tag->tag)->id;
}

