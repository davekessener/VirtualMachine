#include "misc.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "headers.h"

namespace lib
{

bmp_data *load_bmp_from_file(const char *fn)
{
	FILE *f = fopen(fn, "r");

	img::bmp_header_t bmp;
	img::dib_BITMAPINFOHEADER_t dib;

	fread(&bmp, sizeof(bmp), 1, f);
	fread(&dib, sizeof(dib), 1, f);

	bmp_data *d = (bmp_data *) malloc(sizeof(bmp_data) + dib.width * dib.height * dib.bpp / 8 - 1);
	d->width = dib.width;
	d->height = dib.height;
	fread(d->data, dib.bpp / 8, dib.width * dib.height, f);
	
	fclose(f);

	for(size_t i = 0 ; i < dib.width * dib.height ; ++i)
	{
		unsigned char b(d->data[i * 3]);
		d->data[i * 3] = d->data[i * 3 + 2];
		d->data[i * 3 + 2] = b;
	}

	unsigned char *buf = (unsigned char *) malloc(dib.width * 3);
	for(size_t i = 0, N = dib.width * 3 ; i < dib.height / 2 ; ++i)
	{
		memcpy(buf, d->data + N * i, N);
		memcpy(d->data + N * i, d->data + (dib.height - i - 1) * N, N);
		memcpy(d->data + N * (dib.height - i - 1), buf, N);
	}
	free(buf);

	return d;
}

}

