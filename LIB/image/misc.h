#ifndef LIB_IMG_MISC_H
#define LIB_IMG_MISC_H

namespace lib
{
	struct bmp_data
	{
		int width, height;
		unsigned char data[1];
	};

	bmp_data *load_bmp_from_file(const char *);
}

#endif

