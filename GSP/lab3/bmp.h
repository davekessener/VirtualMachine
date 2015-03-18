#ifndef DAV_GSP_2_BMP_H
#define DAV_GSP_2_BMP_H

#include "common.h"

#define BMP_MAGIC ((WORD) (('M' << 8) | 'B'))

typedef struct
{
	WORD red : 5;
	WORD green : 5;
	WORD blue : 5;
} RGB16;

typedef struct
{
	BYTE red;
	BYTE green;
	BYTE blue;
} RGB24;

typedef struct
{
	BYTE red;
	BYTE green;
	BYTE blue;
	BYTE alpha;
} RGBA;

typedef struct
{
	WORD header;
	DWORD size;
	WORD reserved1;
	WORD reserved2;
	DWORD offset;
} __attribute__((packed)) BITMAPFILEHEADER;

typedef struct
{
	DWORD size;
	WORD width;
	WORD height;
	WORD planes;
	WORD bpp;
} BITMAPCOREHEADER;

typedef struct
{
	DWORD size;
	int32_t width;
	int32_t height;
	WORD planes;
	WORD bpp;
	DWORD compression;
	DWORD data_length;
	struct
	{
		int32_t x;
		int32_t y;
	} resolution;
	DWORD colors;
	DWORD important_colors;
} __attribute__((packed)) BITMAPINFOHEADER;

enum
{
	BI_RGB,
	BI_RLE8,
	BI_RLE3,
	BI_BITFIELDS,
	BI_JPEG,
	BI_PNG,
	BI_ALPHABITFIELDS,
	BI_CMYK = 11,
	BI_CMYKRLE8,
	BI_CMYKTLE4
}

#endif

