#include <png/png.hpp>
#include "png.h"

void load_png(std::vector<BYTE>& vec, const std::string& fn, int *pw, int *ph, int *pp)
{
	png::image<png::rgba_pixel> img(fn);
	int w = img.get_width();
	int h = img.get_height();
	int p = 1;

	while(p < w || p < h) p <<= 1;

	vec.resize(p * p * 4);

	for(int y = 0 ; y < h ; ++y)
	{
		for(int x = 0 ; x < w ; ++x)
		{
			auto c = img.get_pixel(x, y);
			vec[(x + y * p) * 4 + 0] = c.alpha;
			vec[(x + y * p) * 4 + 1] = c.blue;
			vec[(x + y * p) * 4 + 2] = c.green;
			vec[(x + y * p) * 4 + 3] = c.red;
		}
	}

	if(pw) *pw = w;
	if(ph) *ph = h;
	if(pp) *pp = p;
}

