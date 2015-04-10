#ifndef DAV_C_NBT_H
#define DAV_C_NBT_H

extern "C" {

#define ERR_NBT_NONE 0

#include "common.h"

typedef struct
{
	BYTE id;
	void *tag;
	char *name;
} NBTTag;

typedef struct
{
	NBTTag *tag;
	uint *rc;
} NBT_ptr;

typedef struct
{
	NBT_ptr *tags;
	uint i, c;
} NBT_TAGS;

typedef struct
{
	void *data;
	uint i, c;
	BYTE type;
} NBT_Array;

typedef struct
{
	void *data;
	BYTE type;
} NBT_Single;

enum
{
	TAG_NONE = 0,
	TAG_BYTE,
	TAG_SHORT,
	TAG_INT,
	TAG_LONG,
	TAG_FLOAT,
	TAG_DOUBLE,
	TAG_BYTE_ARRAY,
	TAG_STRING,
	TAG_LIST,
	TAG_COMPOUND,
	TAG_INT_ARRAY
};

const char *nbt_error_msg(void);
void nbt_clear_error(void);
int nbt_error(void);

NBTTag *read_nbt_from_file(FILE *);
NBTTag *read_nbt(const BYTE *);
NBTTag *nbt_create(BYTE, const char *);
void write_nbt_to_file(FILE *, const NBTTag *);
void nbt_delete(NBTTag *);

BYTE nbt_get_id(NBTTag *);
const char *nbt_get_name(NBTTag *);

void nbt_compound_set_byte(NBTTag *, const char *, BYTE);
BYTE nbt_compound_get_byte(NBTTag *, const char *);
void nbt_compound_set_short(NBTTag *, const char *, SHORT);
SHORT nbt_compound_get_short(NBTTag *, const char *);
void nbt_compound_set_int(NBTTag *, const char *, INT);
INT nbt_compound_get_int(NBTTag *, const char *);
void nbt_compound_set_long(NBTTag *, const char *, LONG);
LONG nbt_compound_get_long(NBTTag *, const char *);
void nbt_compound_set_float(NBTTag *, const char *, FLOAT);
FLOAT nbt_compound_get_float(NBTTag *, const char *);
void nbt_compound_set_double(NBTTag *, const char *, DOUBLE);
DOUBLE nbt_compound_get_double(NBTTag *, const char *);

}

#endif

