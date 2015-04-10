#include "nbt.h"

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

NBT_ptr *clone(NBT_ptr *p)
{
	NBT_ptr *r = malloc(sizeof(NBT_ptr));

	r->tag = p->tag;
	r->rc  = p->rc;

	++*r->rc;

	return r;
}

void delete(NBT_ptr *p)
{
	if(!--*p->rc)
	{
		nbt_delete(p->tag);

		free(p->rc);
	}

	free(p);
}

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
	tag->c = tag->i = c;
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

NBT_TAGS *read_list(wrapper_t *in)
{
	NBT_TAGS *tag = malloc(sizeof(NBT_TAGS));

	BYTE id;
	DWORD l;

	wrapper_read(in, sizeof(id), &id);
	wrapper_read(in, sizeof(l), &l);

	tag->i = tag->c = l;
	tag->tags = NULL;

	if(l)
	{
		uint i;

		tag->tags = malloc(l * sizeof(NBT_ptr));

		for(i = 0 ; i < l ; ++i)
		{
			
		}
	}
}

NBT_TAGS *read_compound(wrapper_t *in)
{
	NBT_TAGS *tag = malloc(sizeof(NBT_TAGS));

	tag->i = tag->c = 0;
	tag->tags = NULL;

	if(in)
	{
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

NBTTag *read_nbt_from_file(FILE *);
NBTTag *read_nbt(const BYTE *);

NBTTag *nbt_create(BYTE id, const char *name)
{
	NBTTag *tag = malloc(sizeof(NBTTag));

	tag->id = id;
	tag->name = strcpy(name);
	tag->tag = read_payload(id, NULL);
}

void write_nbt_to_file(FILE *, const NBTTag *);
void nbt_delete(NBTTag *);

BYTE nbt_get_id(NBTTag *);
const char *nbt_get_name(NBTTag *);

