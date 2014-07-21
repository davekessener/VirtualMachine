#ifndef IMG_H
#define IMG_H

#include <stdint.h>

#define BM_HEADER_MAGIC 0x4D42 // 'BM'
#define BM_RES 0x0B13 // 2,835 px/m
#define BM_BPP 32 // RGBA

#define RGBA_A(v) ((((RGBA)(v))>>24)&0xff)
#define RGBA_R(v) ((((RGBA)(v))>>16)&0xff)
#define RGBA_G(v) ((((RGBA)(v))>> 8)&0xff)
#define RGBA_B(v) ( ((RGBA)(v))     &0xff)
#define RGBA(r,g,b,a) ((RGBA)(((((uint32_t)(a))&0xff)<<24)| \
							  ((((uint32_t)(r))&0xff)<<16)| \
							  ((((uint32_t)(g))&0xff)<< 8)| \
							  ( ((uint32_t)(b))&0xff)     ))
#define TOOFFSET(v) ((*((::IMG::OFFSET *) (v)))&0x00ffffff)

namespace IMG
{
	struct bitmap_header_t
	{
		uint16_t header;
		uint32_t file_size;
		struct
		{
			uint16_t v1, v2;
		} reserved;
		uint32_t offset;
	} __attribute__((packed));
	
	struct DIB_t
	{
		uint32_t header_size;
		int32_t width;
		int32_t height;
		uint16_t color_planes;
		uint16_t bpp;
		uint32_t compression;
		uint32_t size;
		struct
		{
			int32_t horizontal;
			int32_t vertical;
		} resolution;
		uint32_t palette_count;
		uint32_t important_colors;
	} __attribute__((packed));
	
	typedef uint32_t RGBA;
	typedef uint8_t BYTE;
	typedef uint16_t WORD;
	typedef uint32_t DWORD;
	typedef uint64_t QWORD;
	typedef uint32_t OFFSET;
}

#endif

