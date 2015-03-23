#include "bmp.h"

int bmp_bitmapinfoheader_expand(BITMAP *, void **, int);

int bmp_expand(BITMAP *bmp, void **data, int format)
{
	switch(*((DWORD *) bmp->dib.header))
	{
		case sizeof(BITMAPINFOHEADER):
			return bmp_bitmapinfoheader_expand(bmp, data, format);
		default:
			return BMP_ERR_UNSUPPORTED;
	}
}

int bmp_compress(BITMAP *bmp, const void *data, int format)
{
}

// # ===========================================================================

size_t calc_bufsize(int f, size_t w, size_t h)
{
	return f * w * h;
}

void set_color(BYTE *p, uint o, int f, RGBA c)
{
	p += o * f;

	switch(f)
	{
		case BMP_RGB16:
			((RGB16 *) p)->red = c.red >> 3;
			((RGB16 *) p)->green = c.green >> 3;
			((RGB16 *) p)->blue = c.blue >> 3;
			break;
		case BMP_RGB24:
			((RGB24 *) p)->red = c.red;
			((RGB24 *) p)->green = c.green;
			((RGB24 *) p)->blue = c.blue;
			break;
		case BMP_RGBA:
			*((RGBA *) p) = c;
			break;
		default:
			assert(!"This shouldn't happen!");
	}
}

typedef struct
{
	const BYTE *d;
	uint w, h;
	uint bpp;
} color_reader_t;

int bmp_expand_rgb(const BYTE *raw, int32_t width, int32_t height, const RGBA *pallet, WORD bpp, void **data, int format)
{
#define ABS(a) ((a)<0?-(a):(a))
	uint w = ABS(width);
	uint h = ABS(height);
	int32_t x, y;
	int32_t dx = width < 0 ? -1 : 1;
	int32_t dy = height < 0 ? -1 : 1;
	BYTE *buf = malloc(calc_bufsize(format, w, h));
	color_reader_t cr;
#undef ABS

	cr_init(&cr, raw, w, h, bpp);

#define MAX(a,b) ((a)>(b)?(a):(b))
	for(y = MAX(1, height) - 1 ; y != MAX(-1, -height) ; y -= dy)
	{
		for(x = MAX(1, -width) - 1 ; x != MAX(-1, width) ; x += dx)
		{
			set_color(buf, x + y * w, format, cr_get_color(&cr));
		}
	}
#undef MAX
}

int bmp_bitmapinfoheader_expand(BITMAP *bmp, void **data, int format)
{
	BITMAPINFOHEADER *dib = (BITMAPINFOHEADER *) bmp->dib.header;

	switch(dib->compression)
	{
		case BI_RGB:
			return bmp_expand_rgb (bmp->dib.raw, dib->width, dib->height, bmp->dib.pallet, bmp->dib.bpp, data, format);
		case BI_RLE4:
			return bmp_expand_rle4(bmp->dib.raw, dib->width, dib->height, bmp->dib.pallet, data, format);
		case BI_RLE8:
			return bmp_expand_rle8(bmp->dib.raw, dib->width, dib->height, bmp->dib.pallet, data, format);
		default:
			return BMP_ERR_UNSUPPORTED;
	}

	return BMP_ERR_NO;
}

