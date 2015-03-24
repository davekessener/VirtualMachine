#include "bmp.h"

uint calculate_skip(uint, uint, uint);

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
	const RGBA *p;
	uint w, h;
	uint bpp;
	uint i, s;
} color_reader_t;

void cr_init(color_reader_t *self, const BYTE *d, const RGBA *p, uint w, uint h, uint bpp)
{
	self->d = d;
	self->p = p;
	self->w = w;
	self->h = h;
	self->bpp = bpp;
	self->i = 0;
	self->s = calculate_skip(bpp, w, h);
}

RGBA cr_get_color(color_reader_t *self)
{
	if(!(self->i % self->w)) self->d += self->s;

	switch(self->bpp)
	{
		case 1:
		case 2:
		case 4:
			{
				uint c = self->bpp / 8;
				uint sh = (c - (self->i % c)) % c;
				uint v = *self->d >> sh;
				if(!(++self->i % c)) ++self->d;
				return self->p[v];
			}
			break;
		case 8:
			++self->c;
			return self->p[*self->d++];
		case 16:
			{
				RGB16 v = *((RGB16 *) self->d);
				RGBA c = {
					.red   = v.red   << 3,
					.green = v.green << 3,
					.blue  = v.blue  << 3,
					.alpha = 0
				};
				self->d += sizeof(RGB16);
				++self->c;
				return c;
			}
		case 24:
			{
				RGB24 v = *((RGB24 *) self->d);
				RGBA c = {.red = v.red, .green = v.green, .blue = v.blue, .alpha = 0};
				self->d += sizeof(RGB24);
				++self->c;
				return c;
			}
		case 32:
			++self->c;
			self->d += sizeof(RGBA);
			return self->d[-1];
		default:
			assert(!"This shouldn't happen either.");
	}
}

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

	cr_init(&cr, raw, w, h, bpp, pallet);

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

uint calculate_skip(uint bpp, uint width, uint height)
{
#define ABS(a) ((a)<0?-(a):(a))
#define DIVROUNDUP(a,b) (((a)+((b)-1))/(b))
	size_t w = ABS(width);
	size_t h = ABS(height);
	size_t ws = DIVROUNDUP(bpp * w, 8);
	size_t rs = DIVROUNDUP(bpp * w, sizeof(DWORD) * 8) * 4;
#undef DIVROUNDUP
#undef ABS

	return rs - ws;
}

