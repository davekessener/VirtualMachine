template<class T>
basic_image<T>::basic_image(int width, int height) : width(width), height(height)
{
	image = (width * height > 0) ? new T[width * height] : NULL;
	if(image)
	{
		memset(image, 0, width * height * sizeof(T));
	}
}

template<class T>
basic_image<T>::basic_image(const basic_image<T>& img) : width(img.width), height(img.height)
{
	image = img.image ? new T[width * height] : NULL;
	if(image)
	{
		T *pT1 = image, *pT2 = img.image;
		for(int i = width * height ; i-- ;)
		{
			*pT1++ = *pT2++;
		}
	}
}

template<class T>
void basic_image<T>::toFile(std::fstream& f) const
{
	if(image == NULL) return;

	bitmap_header_t bmh;
	DIB_t dib;

	prepareDIB(&dib);
	prepareBMH(&bmh, &dib);

	f.write((const char *) &bmh, sizeof(bitmap_header_t));
	f.write((const char *) &dib, sizeof(DIB_t));

	write(f);
}

template<class T>
void basic_image<T>::write(std::fstream& f) const
{
	uint32_t b = (width * sizeof(T)) % 4, e = 0;
	if(b > 0)
	{
		b = 4 - b;

		for(int i = 0 ; i < height ; i++)
		{
			f.write((const char *) (image + i * width), width * sizeof(T));
			f.write((const char *) &e, b);
		}
	}
	else
	{
		f.write((const char *) image, width * height * sizeof(T));
	}
}

template<class T>
basic_image<T>& basic_image<T>::bitBlt(int x, int y, int dx, int dy, const basic_image<T>& source, int sx, int sy, T transcolor, bool transparent)
{
	if(image == NULL) return *this;

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
			T t = source.image[(sy + sdy + ry * ty) * source.width + sx + sdx + rx * tx];

			if(!transparent || !(t == transcolor))
			{
				image[(y + ty) * width + x + tx] = t;
			}
		}
	}

	return *this;
}

template<class T>
basic_image<T>& basic_image<T>::trunc(int x, int y, int dx, int dy)
{
	if(image == NULL) return *this;

	if(x < 0) x = 0;
	if(y < 0) y = 0;
	if(dx < 0) dx = width - x;
	if(dy < 0) dy = height - y;

	T *tmp = new T[dx * dy];

	for(int ty = 0 ; ty < dy ; ty++)
	{
		memcpy(tmp + ty * dx, image + (ty + y) * width + x, dx * sizeof(T));
	}

	delete[] image;

	image = tmp;
	width = dx;
	height = dy;

	return *this;
}

template<class T>
basic_image<T>& basic_image<T>::clear(void)
{
	if(image == NULL) return *this;

	memset(image, 0, width * height * sizeof(T));

	return *this;
}

template<class T>
basic_image<T>::~basic_image(void)
{
	delete[] image;
}

template<class T>
int basic_image<T>::Size(void) const
{
	return width * height * sizeof(T);
}

// # ---------------------------------------------------------------------------

template<class T>
void basic_image<T>::prepareBMH(bitmap_header_t *bmh, DIB_t *dib) const
{
	bmh->header = BM_HEADER_MAGIC;
	bmh->reserved = {0, 0};
	bmh->offset = sizeof(bitmap_header_t) + sizeof(DIB_t);
	bmh->file_size = bmh->offset + dib->size;
}

template<class T>
void basic_image<T>::prepareDIB(DIB_t *dib) const
{
	dib->header_size = sizeof(DIB_t);
	dib->width = width;
	dib->height = -height;
	dib->color_planes = 1;
	dib->bpp = sizeof(T) * 8;
	dib->compression = 0;
	dib->size = ((width * dib->bpp + 31) >> 5) * height * 4;
	dib->resolution = {BM_RES, BM_RES};
	dib->palette_count = 0;
	dib->important_colors = 0;
}

