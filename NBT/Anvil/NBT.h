#ifndef NBT_H
#define NBT_H

#include <stdint.h>
#include <string.h>
#include <assert.h>

enum
{
	NBT_NOTHING,
	NBT_BYTE,
	NBT_SHORT,
	NBT_INT,
	NBT_LONG,
	NBT_FLOAT,
	NBT_DOUBLE,
	NBT_BYTE_ARRAY,
	NBT_STRING,
	NBT_LIST,
	NBT_COMPOUND,
	NBT_INT_ARRAY
};

typedef struct
{
	const char *name;
	BYTE type;
	void *data;
} NBT;

typedef struct
{
	void *data;
	int c, i;
} NBT_Array;

typedef struct
{
	BYTE type;
	void **data;
	int c, i;
} NBT_List;

typedef struct
{
	NBT *tags;
	int c, i;
} NBT_Compound;

NBT *nbt_read(FILE *);
void nbt_write(const NBT *, FILE *);

#endif

