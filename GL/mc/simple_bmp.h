#ifndef LIB_IMG_SIMPLEBMP_H
#define LIB_IMG_SIMPLEBMP_H

#include <vector>
#include "inc.h"

namespace lib
{
	namespace img
	{
		namespace simple
		{
			struct image
			{
				int width, height, bpp;
				std::vector<DWORD> data;
			};

			image read_simple_bmp(const std::string&, bool = true);

			inline BYTE *raw(image& i) { return reinterpret_cast<BYTE *>(&*i.data.begin()); }
		}
	}
}

#endif

