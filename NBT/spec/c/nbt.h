#ifndef DAV_C_NBT_H
#define DAV_C_NBT_H

//extern "C" {

#define ERR_NBT_NONE 0

#include <stdio.h>
#include <stdlib.h>
#include "common.h"

typedef struct
{
	BYTE id;
	void *tag;
	char *name;
} NBTTag;

typedef struct
{
	NBTTag **tags;
	uint i, c;
} NBT_Compound;

typedef struct
{
	BYTE id;
	void **payloads;
	uint i, c;
} NBT_List;

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
NBTTag *nbt_create(BYTE, const char *, void *);
void write_nbt_to_file(FILE *, const NBTTag *);
void nbt_delete(NBTTag *);

BYTE nbt_get_id(NBTTag *);
const char *nbt_get_name(NBTTag *);

void nbt_compound_set_byte(NBTTag *, const char *, BYTE);
BYTE nbt_compound_get_byte(const NBTTag *, const char *);
void nbt_compound_set_short(NBTTag *, const char *, WORD);
WORD nbt_compound_get_short(const NBTTag *, const char *);
void nbt_compound_set_int(NBTTag *, const char *, DWORD);
DWORD nbt_compound_get_int(const NBTTag *, const char *);
void nbt_compound_set_long(NBTTag *, const char *, QWORD);
QWORD nbt_compound_get_long(const NBTTag *, const char *);
void nbt_compound_set_float(NBTTag *, const char *, float);
float nbt_compound_get_float(const NBTTag *, const char *);
void nbt_compound_set_double(NBTTag *, const char *, double);
double nbt_compound_get_double(const NBTTag *, const char *);
void nbt_compound_set_string(NBTTag *, const char *, const char *);
const char *nbt_compound_get_string(const NBTTag *, const char *);
void nbt_compound_set_byte_array(NBTTag *, const char *, const BYTE *, const BYTE *);
const BYTE *nbt_compound_get_byte_array(const NBTTag *, const char *, DWORD *);
void nbt_compound_set_int_array(NBTTag *, const char *, const DWORD *, const DWORD *);
const DWORD *nbt_compound_get_int_array(const NBTTag *, const char *, DWORD *);

void nbt_compound_set_tag(NBTTag *, const char *, NBTTag *);
NBTTag *nbt_compound_get_tag(NBTTag *, const char *);
void nbt_compound_set_compoundtag(NBTTag *, const char *, NBTTag *);
NBTTag *nbt_compound_get_compoundtag(NBTTag *, const char *);
void nbt_compound_set_list(NBTTag *, const char *, NBTTag *);
NBTTag *nbt_compound_get_list(NBTTag *, const char *);

void nbt_list_add_tag(NBTTag *, NBTTag *);
void *nbt_list_begin(NBTTag *);
void *nbt_list_end(NBTTag *);
size_t nbt_list_size(const NBTTag *);
BYTE nbt_list_type(const NBTTag *);

//}

#endif

