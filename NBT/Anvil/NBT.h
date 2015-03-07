#ifndef NBT_H
#define NBT_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

typedef uint8_t BYTE;
typedef uint16_t WORD;
typedef uint32_t DWORD;
typedef uint64_t QWORD;

enum
{
	NBT_NOTHING = 0,
	NBT_BYTE = 1,
	NBT_SHORT = 2,
	NBT_INT = 3,
	NBT_LONG = 4,
	NBT_FLOAT = 5,
	NBT_DOUBLE = 6,
	NBT_BYTE_ARRAY = 7,
	NBT_STRING = 8,
	NBT_LIST = 9,
	NBT_COMPOUND = 10,
	NBT_INT_ARRAY = 11
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
	NBT **tags;
	int c, i;
} NBT_Compound;

NBT *nbt_read(FILE *);
void nbt_write(const NBT *, FILE *);
void nbt_delete(NBT *);

#endif

