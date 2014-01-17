#include "Palette.h"

using namespace IMG;

Palette::Palette(int size) : size((unsigned) size)
{
	palette = size > 0 ? new RGBA[size] : NULL;

	if(palette != NULL)
	{
		memset(palette, 0, size * sizeof(RGBA));
	}
}

Palette::Palette(RGBA *data, int size) : size(size)
{
	palette = new RGBA[size];
	memcpy(palette, data, size * sizeof(RGBA));
}

Palette::Palette(const Palette& pal) : size(pal.size)
{
	palette = size > 0 ? new RGBA[size] : NULL;

	if(size > 0)
	{
		memcpy(palette, pal.palette, size * sizeof(RGBA));
	}
}

Palette::~Palette(void)
{
	delete[] palette;
}

Palette& Palette::setColor(int i, RGBA c)
{
#ifdef DEBUG
	if(i < 0 || i >= size) 
		std::cerr << "ERR: Invalid index " << i << " in write; palette of size " << 
			size << "." << std::endl << "Abort." << std::endl;
	assert(palette);
#endif
	
	palette[i] = c;

	return *this;
}

Palette& Palette::setColor(RGBA *cs, int i, int l)
{
#ifdef DEBUG
	if(i < 0 || i + l > size)
		std::cerr << "ERR: Invalid color insertion: " << l << "@" << i << 
			" in palette of size " << size << "." << std::endl << "Abort." << std::endl;
	assert(palette);
#endif

	if(l < 0) l = size - i;

	memcpy(palette + i, cs, l * sizeof(RGBA));

	return *this;
}

RGBA Palette::get(int i) const
{
#ifdef DEBUG
	if(i < 0 || i >= size)
		std::cerr << "ERR: Invalid index " << i << " in read; palette of size " <<
			size << "." << std::endl << "Abort." << std::endl;
	assert(palette);
#endif

	return palette[i];
}

RGBA& Palette::operator[](int i)
{
#ifdef DEBUG
	if(i < 0 || i >= size)
		std::cerr << "ERR: Invalid index " << i << " in read; palette of size " <<
			size << "." << std::endl << "Abort." << std::endl;
	assert(palette);
#endif

	return palette[i];
}

RGBA Palette::operator[](int i) const
{
	return get(i);
}

Palette& Palette::operator=(const Palette& p)
{
	size = p.size;
	palette = size > 0 ? new RGBA[size] : NULL;

	if(size > 0)
	{
		memcpy(palette, p.palette, size * sizeof(RGBA));
	}
}

int Palette::Size(void) const
{
	return size;
}

