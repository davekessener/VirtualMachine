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
//		case BI_RLE4:
//			if(bmp->bpp != 4)
//				return BMP_ERR_INVALID;
//			break;
		default:
			return BMP_ERR_NOTSUPPORTED;
	}
	
	switch(bmp->bpp)
	{
//		case 1:
//		case 4:
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

int read_raw_buffer(FILE *f, BYTE **data, uint bpp, uint dl, uint w, uint h)
{
#define MXT_DIVROUNDUP(a,b) (((a)+((b)-1))/(b))
	uint ars = MXT_DIVROUNDUP(bpp * w, 8);
	uint rs = MXT_DIVROUNDUP(bpp * w, sizeof(DWORD) * 8) * 4;
	uint ts = rs * h;
	BYTE *buf = NULL;
	int y;
#undef MXT_DIVROUNDUP

	if(dl && ts != dl)
		return BMP_ERR_INVALID;
	
	buf = malloc(ts);

	if(fread(buf, sizeof(BYTE), ts, f) != ts)
	{
		free(buf);
		return BMP_ERR_TOOSHORT;
	}

	*data = malloc(h * ars);

	for(y = 0 ; y < h ; ++y)
	{
		memcpy((BYTE *)data + y * ars, buf + y * rs, ars);
	}

	free(buf);

	return BMP_ERR_NO;
}

uint read_pixel(const void *, WORD, RGBA *);

int bmp_read_raw(FILE *f, BITMAPINFOHEADER *bmp, RGBA **buffer)
{
#define MXT_ABS(v) ((v)<0?-(v):(v))
	DWORD w = MXT_ABS(bmp->width);
	DWORD h = MXT_ABS(bmp->height);
	RGBA *buf = NULL;
	RGBA *img = NULL;
	int x, y;
	int dx = bmp->width < 0 ? -1 : 1;
	int dy = bmp->height < 0 ? -1 : 1;
#undef MXT_ABS

	if((x = read_raw_buffer(f, &buf, bmp->bpp, bmp->data_length, w, h)))
		return x;

	img = malloc(w * h * sizeof(RGBA));

#define MXT_MAX(a,b) ((a)>(b)?(a):(b))
	for(y = MXT_MAX(1, bmp->height) - 1 ; y != MXT_MAX(-1, -bmp->height) ; y -= dy)
	{
		for(x = MXT_MAX(1, -bmp->width) - 1 ; x != MXT_MAX(-1, bmp->width) ; x += dx)
		{
			buf += read_pixel(buf, bmp->bpp, img + (x + y * w));
		}
	}
#undef MXT_MAX

	*buffer = img;
	free(buf);
	
	return BMP_ERR_NO;
}

int bmp_read_pallet(FILE *f, BITMAPINFOHEADER *bmp, RGBA **buffer)
{
#define MXT_ABS(v) ((v)<0?-(v):(v))
	uint pc = bmp->colors ? bmp->colors : (1 << bmp->bpp);
	uint w = MXT_ABS(bmp->width);
	uint h = MXT_ABS(bmp->height);
	int dx = bmp->width < 0 ? -1 : 1;
	int dy = bmp->height < 0 ? -1 : 1;
	RGBA *pallet = NULL;
	RGBA *img = NULL;
	BYTE *buf = NULL;
	BYTE *rbuf = NULL;
#undef MXT_ABS

	pallet = malloc(sizeof(RGBA) * pc);
	if(fread(pallet, sizeof(RGBA), pc, f) != pc * sizeof(RGBA))
	{
		free(pallet);
		return BMP_ERR_TOOSHORT;
	}

	if((x = read_raw_buffer(f, &rbuf, bmp->bpp, bmp->data_length, w, h)))
	{
		free(pallet);
		return x;
	}

	if(bmp->compression != BI_RGB)
	{
		buf = malloc(w * h);
		memset(buf, 0, w * h);
		if(decompress_rle(rbuf, , buf, w, h))
		{
			free(pallet);
			free(rbuf);
			free(buf);
			return BMP_ERR_DEOMPRESSION;
		}
		free(rbuf);
	}
	else
	{
		assert(w * h * bmp->bpp / 8 == bmp->data_length);
		buf = rbuf;
	}

	img = malloc(sizeof(RGBA) * w * h);
	rbuf = buf;

#define MXT_MAX(a,b) ((a)>(b)?(a):(b))
	for(y = MXT_MAX(1, bmp->height) - 1 ; y != MXT_MAX(-1, -bmp->height) ; y -= dy)
	{
		for(x = MXT_MAX(1, -bmp->width) - 1 ; x != MXT_MAX(-1, bmp->width) ; x += dx)
		{
			img[x + y * w] = pallet[*rbuf++];
		}
	}
#undef MXT_MAX

	free(pallet);
	free(buf);

	*buffer = img;

	return BMP_ERR_NO;
}

uint read_pixel(const void *d, WORD bpp, RGBA *t)
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

