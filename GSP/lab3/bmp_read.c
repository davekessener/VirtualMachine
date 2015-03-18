#include "bmp_read.h"

int bmp_read_raw(FILE *, BITMAPINFOHEADER *, RGBA **);
int bmp_read_pallet(FILE *, BITMAPINFOHEADER *, RGBA **);

int bmp_read(FILE *f, BITMAPINFOHEADER *bmp, RGBA **buffer)
{
	BITMAPFILEHEADER fh;

	if(fread(&fh, sizeof(fh), 1, f) != sizeof(fh))
		return BMP_ERR_TOOSHORT;

	if(fh.header != BMP_MAGIC)
		return BMP_ERR_INVALID;

	if(fread(bmp, sizeof(*bmp), 1, f) != sizeof(*bmp))
		return BMP_ERR_TOOSHORT;

	if(bmp->size != sizeof(BITMAPFILEHEADER))
		return BMP_ERR_NOTSUPPORTED;

	switch(bmp->compression)
	{
		case BI_RGB:
			break;
		case BI_RLE8:
			if(bmp->bpp != 8)
				return BMP_ERR_INVALID;
			break;
		case BI_RLE4:
			if(bmp->bpp != 4)
				return BMP_ERR_INVALID;
			break;
		default:
			return BMP_ERR_NOTSUPPORTED;
	}
	
	switch(bmp->bpp)
	{
		case 1:
		case 4:
		case 8:
			return bmp_read_pallet(f, bmp, buffer);
		case 16:
		case 24:
		case 32:
			return bmp_read_raw(f, bmp, buffer);
		default:
			return BMP_ERR_INVALID;
	}
}

uint read_raw(const void *, WORD, RGBA *);

int bmp_read_raw(FILE *f, BITMAPINFOHEADER *bmp, RGBA **buffer)
{
#define MXT_ABS(v) ((v)<0?-(v):(v))
#define MXT_DIVROUNDUP(a,b) (((a)+((b)-1))/(b))
	DWORD w = MXT_ABS(bmp->width);
	DWORD h = MXT_ABS(bmp->height);
	size_t rs = MXT_DIVROUNDUP(bmp->bpp * w, sizeof(DWORD) * 8) * 4;
	size_t ts = rs * h;
	RGBA *buf = NULL;
	RGBA *img = NULL;
	int x, y;
	int dx = bmp->width < 0 ? -1 : 1;
	int dy = bmp->height < 0 ? -1 : 1;
	uint s = rs - w * bmp->bpp / 8;
#undef MXT_DIVROUNDUP
#undef MXT_ABS

	if(bmp->data_length && ts != bmp->data_length)
		return BMP_ERR_INVALID;
	
	buf = malloc(ts);

	if(fread(buf, sizeof(BYTE), ts, f) != ts)
	{
		free(buf);
		return BMP_ERR_TOOSHORT;
	}

	img = malloc(w * h * sizeof(RGBA));

#define MXT_MAX(a,b) ((a)>(b)?(a):(b))
#define MXT_MIN(a,b) ((a)<(b)?(a):(b))
	for(y = MXT_MAX(1, -bmp->height) - 1 ; y != MXT_MAX(-1, bmp->height) ; y += dy)
	{
		for(x = MXT_MAX(1, -bmp->width) - 1 ; x != MXT_MAX(-1, bmp->width) ; x += dx)
		{
			buf += read_raw(buf, bmp->bpp, img + (x + y * w));
		}

		buf += s;
	}
#undef MXT_MIN
#undef MXT_MAX

	*buffer = img;
	free(buf);
	
	return BMP_ERR_NO;
}

int bmp_read_pallet(FILE *f, BITMAPINFOHEADER *bmp, RGBA **buffer)
{
	return BMP_ERR_NO;
}

uint read_raw(const void *d, WORD bpp, RGBA *t)
{
	t->alpha = 0;

	switch(bpp)
	{
		case 16:
			t->red   = ((RGB16 *) d)->red;
			t->green = ((RGB16 *) d)->green;
			t->blue  = ((RGB16 *) d)->blue;
			return sizeof(RGB16);
		case 24:
			t->red   = ((RGB24 *) d)->red;
			t->green = ((RGB24 *) d)->green;
			t->blue  = ((RGB24 *) d)->blue;
			return sizeof(RGB24);
		case 32:
			*t = *((RGBA *) d);
			return sizeof(RGBA);
	}

	return 0;
}

