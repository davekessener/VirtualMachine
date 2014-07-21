#include "palette_image.h"

using namespace IMG;

palette_image::palette_image(int width, int height, int _bpp) : 
	basic_image<uint8_t>(width, height), 
	bpp((_bpp != 1 && _bpp != 4 && _bpp != 8) ? 8 : _bpp)
{
	palette = Palette(1 << bpp);
}

palette_image::palette_image(const LZ77& lz77) : basic_image<uint8_t>(8, lz77.s * 2 / 8), palette(), bpp(4)
{
	memcpy(image + lz77.s, lz77.d, lz77.s);

	for(int i = 0 ; i < lz77.s ; i++)
	{
		image[i * 2]     =  image[lz77.s + i] & 0x0f;
		image[i * 2 + 1] = (image[lz77.s + i] & 0xf0) >> 4;
	}
}

palette_image::palette_image(const palette_image& pimg) : basic_image<uint8_t>(pimg), palette(pimg.palette), bpp(pimg.bpp)
{
}

void palette_image::toFile(std::fstream& f) const
{
	bitmap_header_t bmh;
	DIB_t dib;

	prepareDIB(&dib);
	prepareBMH(&bmh, &dib);

	dib.bpp = bpp;
	dib.size = ((width * bpp + 31) >> 5) * height * 4;
	bmh.offset += (1 << bpp) * sizeof(RGBA);
	bmh.file_size = bmh.offset + dib.size;

	f.write((const char *) &bmh, sizeof(bitmap_header_t));
	f.write((const char *) &dib, sizeof(DIB_t));
//	fwrite(&bmh, sizeof(bitmap_header_t), 1, f);
//	fwrite(&dib, sizeof(DIB_t), 1, f);

	const RGBA &rref = palette[0];
	f.write((const char *) &rref, palette.Size() * sizeof(RGBA));
//	fwrite(&rref, sizeof(RGBA), palette.Size(), f);

	int i = (1 << bpp) - palette.Size();
	while(i--) f.put(0); //putc(0, f);

	uint8_t *d = new uint8_t[dib.size];
	int bpr = dib.size / height;
	int dif = bpr - (width * bpp + 7) / 8;

	for(int y = 0 ; y < height ; y++)
	{
		int x = 0;
		for(; x < width ; x += 8 / bpp)
		{
			uint8_t v = 0;
			for(int i = 0 ; i < 8 / bpp ; i++)
			{
				if(x + i >= width) break;
				v |= image[y * width + x + i] << (i * (8 / bpp));
			}
			d[y * bpr + x] = v;
		}
//		// Unneccessary, since 'new[]' initalized uint8_t's to 0
//		for(int x = dif ; x < bpr ; x++)
//		{
//			d[y * bpr + x] = 0;
//		}
	}

	f.write((const char *) d, dib.size);
//	fwrite(d, sizeof(uint8_t), dib.size, f);
}

palette_image& palette_image::setPalette(const Palette& pal)
{
	palette = pal;

	return *this;
}

palette_image::operator rgba_image(void) const
{
	rgba_image rgba(width, height);

	for(int y = 0 ; y < height ; y++)
	{
		for(int x = 0 ; x < width ; x++)
		{
			int i = image[y * width + x];
			rgba.image[y * width + x] = i >= palette.Size() ? RGBA(0,0,0,0xff) : palette[i];
		}
	}

	return rgba;
}

