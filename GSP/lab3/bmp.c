#include "bmp.h"

int read_bitmapinfoheader(BITMAP *, FILE *);

size_t calculate_rgb_size(WORD, int32_t, int32_t);

int read_bmp_file(BITMAP *bmp, FILE *f)
{
	DWORD l = 0;

	bmp->dib.header = NULL;
	bmp->dib.raw = NULL;
	bmp->dib.pallet = NULL;

	if(fread(&bmp->header, sizeof(bmp->header), 1, f) != sizeof(bmp->header))
		return BMP_ERR_EOF;

	if(fread(&l, sizeof(l), 1, f) != sizeof(l))
		return BMP_ERR_EOF;

	switch(l)
	{
		case sizeof(BITMAPINFOHEADER):
			return read_bitmapinfoheader(bmp, f);
		default:
			return BMP_ERR_UNSUPPORTED;
	}
}

int write_bmp_file(const BITMAP *bmp, FILE *f)
{
	if(!bmp->dib.header || !bmp->dib)
		return BMP_ERR_INVALID;

	fwrite(&bmp->header, sizeof(bmp->header), 1, f);
	fwrite(bmp->dib.header, *((DWORD *) bmp->dib.header), 1, f);
	if(bmp->dib.pallet) sizeof(RGBA), fwrite(bmp->dib.pallet, bmp->dib.cc, f);
	fwrite(bmp->dib.raw, bmp->dib.fs, 1, f);

	return BMP_ERR_NO;
}

void free_bmp(BITMAP *bmp)
{
	free(bmp->dib.raw);
	free(bmp->dib.header);
	free(bmp->dib.pallet);
	free(bmp);
}

// # ===========================================================================

int read_pallet(BITMAP *bmp, uint cc, FILE *f)
{
	bmp->dib.pallet = malloc(cc * sizeof(RGBA));

	if(fread(bmp->dib.pallet, sizeof(RGBA), cc, f) != cc * sizeof(RGBA))
		return BMP_ERR_EOF;

	bmp->dib.cc = cc;

	return BMP_ERR_NO;
}

size_t calculate_rgb_size(WORD bpp, int32_t width, int32_t height)
{
#define ABS(a) ((a)<0?-(a):(a))
#define DIVROUNDUP(a,b) (((a)+((b)-1))/(b))
	size_t w = ABS(width);
	size_t h = ABS(height);
	size_t rs = MXT_DIVROUNDUP(bpp * w, sizeof(DWORD) * 8) * 4;
	size_t ts = rs * h;
#undef DIVROUNDUP
#undef ABS

	return ts;
}

int read_bitmapinfoheader(BITMAP *bmp, FILE *f)
{
	BITMAPINFOHEADER *dib = malloc(sizeof(BITMAPINFOHEADER));
	const size_t CS = sizeof(*dib) - sizeof(dib->size);
	int tmp = 0;

	bmp->dib.header = dib;

	if(fread(((BYTE *) dib) + sizeof(dib->size), CS, 1, f) != CS)
		return BMP_ERR_EOF;
	
	bmp->dib.fs = dib->data_length ? dib->data_length : calculate_rgb_size(dib->bpp, dib->width, dib->height);
	bmp->dib.cc = dib->colors ? dib->colors : (1 << dib->bpp);

	if(dib->bpp <= 8 && (tmp = read_pallet(bmp, bmp->dib.fs, f)))
		return tmp;
	
	bmp->dib.raw = malloc(bmp->dib.fs);

	if(fread(bmp->dib.raw, bmp->dib.fs, 1, f) != bmp->div.fs)
		return BMP_ERR_EOF;
	
	return BMP_ERR_NO;
}

