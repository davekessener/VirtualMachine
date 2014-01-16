#include "Image.h"

using namespace IMG;

Image::Image(int _w, int _h) : palette(NULL)
{
	width = _w;
	height = _h;

	image = (width * height > 0) ? new uint8_t[width * height] : NULL;
	if(image != NULL)
	{
		memset(image, 0, width * height);
	}
}

Image::Image(const LZ77& lz77)
{
	width = 8;
	height = lz77.s * 2 / 8;

	image = new uint8_t[width * height];

	memcpy(image + lz77.s, lz77.d, lz77.s);

	for(int i = 0 ; i < lz77.s ; i++)
	{
		image[i * 2]     =  image[lz77.s + i] & 0x0f;
		image[i * 2 + 1] = (image[lz77.s + i] & 0xf0) >> 4;
	}
}

Image::Image(const Image& img)
{
	palette = img.palette;
	width = img.width;
	height = img.height;

	image = (img.image == NULL) ? NULL : new BYTE[width * height];
	if(image != NULL)
	{
		memcpy(image, img.image, width * height);
	}
}

Image::~Image(void)
{
	delete[] image;
}

void Image::toFile(FILE *f)
{
	if(image == NULL || palette == NULL) return;

	bitmap_header_t bmh;
	DIB_t dib;

	bmh.header = BM_HEADER_MAGIC;
	bmh.reserved = {0, 0};
	bmh.offset = bmh.file_size = sizeof(bitmap_header_t) + sizeof(DIB_t);

	dib.header_size = sizeof(DIB_t);
	dib.width = width;
	dib.height = -height;
	dib.color_planes = 1;
	dib.bpp = BM_BPP;
	dib.compression = 0;
	dib.size = ((width * dib.bpp + 31) >> 5) * height * 4;
	dib.resolution = {BM_RES, BM_RES};
	dib.palette_count = 0;
	dib.important_colors = 0;

	bmh.file_size += dib.size;

	fwrite(&bmh, sizeof(bitmap_header_t), 1, f);
	fwrite(&dib, sizeof(DIB_t), 1, f);

	for(unsigned int i = 0, j, s = width * height ; i < s ; i++)
	{
		j = palette->get(image[i]);
		fwrite(&j, sizeof(RGBA), 1, f);
	}
}

Image& Image::setPalette(const Palette& p)
{
	palette = &p;

	return *this;
}

Image& Image::setRect(void *d, int x, int y, int dx, int dy)
{
	for(int py = 0 ; py < dy ; py++)
	{
		memcpy(image + (py + y) * width + x, ((BYTE *) d) + py * dx, dx);
	}

	return *this;
}

Image& Image::hMirror(void)
{
	BYTE *buf = new BYTE[width];

	for(int i = 0 ; i < height / 2 ; i++)
	{
		memcpy(buf, image + i * width, width);
		memcpy(image + i * width, image + (height - i - 1) * width, width);
		memcpy(image + (height - i - 1) * width, buf, width);
	}

	delete[] buf;

	return *this;
}

Image& Image::vMirror(void)
{
	for(int x = 0 ; x < width ; x++)
	{
		for(int y = 0 ; y < height / 2 ; y++)
		{
			BYTE t = image[y * width + x];
			image[y * width + x] = image[(height - y - 1) * width + x];
			image[(height - y - 1) * width + x] = t;
		}
	}

	return *this;
}

Image& Image::offsetBy(int o)
{
	BYTE b = (BYTE) o;

	for(int i = 0 ; i < width * height ; i++)
	{
		image[i] = (image[i] & 0x0f) | ((b & 0x0f) << 4);
	}

	return *this;
}

Image& Image::bitBlt(int x, int y, int dx, int dy, const Image& source, int sx, int sy, int transparent)
{
	int sdx = dx < 0 ? -dx - 1 : 0, sdy = dy < 0 ? -dy - 1 : 0;
	int  rx = dx < 0 ? -1 : 1,   ry = dy < 0 ? -1 : 1;
	if(dx < 0) dx = -dx;
	if(dy < 0) dy = -dy;
#ifdef DEBUG
	if(y * width + x < 0 || (y + dy - 1) * width + x + dx - 1 >= width * height)
	{
		std::cerr << "ERR: Image::bitBlt out of bounds: @(" << x << "|" << y << ") with size " <<
			dx << "/" << dy << std::endl << "     Target size: " << width << "/" << height <<
			std::endl << "     Source size: " << source.width << "/" << source.height << 
			" @(" << sx << "|" << sy << ")" << std::endl;
	}
#endif
	for(int ty = 0 ; ty < dy ; ty++)
	{
		for(int tx = 0 ; tx < dx ; tx++)
		{
			BYTE t = source.image[(sy + sdy + ry * ty) * source.width + sx + sdx + rx * tx];

			if(transparent < 0 || transparent != t)
			{
				image[(y + ty) * width + x + tx] = t;
			}
		}
	}

	return *this;
}

void Image::clear(void)
{
	memset(image, 0, width * height);
}

int Image::getWidth(void) const
{
	return width;
}

int Image::getHeight(void) const
{
	return height;
}

int Image::getSize(void) const
{
	return width * height;
}

