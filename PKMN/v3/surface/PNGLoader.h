#ifndef PKMN_EDITOR_SURFACE_PNGLOADER_H
#define PKMN_EDITOR_SURFACE_PNGLOADER_H

#include <string>
#include <vector>
#include "common.h"

namespace dav
{
	namespace pkmn
	{
		namespace screen
		{
			class PNGLoader
			{
				public:
					struct raw_info
					{
						int w, h;
						int s;
					};
				public:
					static DWORD LoadSquarePNG(const std::string&);
					static text_info LoadPNG(const std::string&);
					static raw_info LoadRawPNG(const std::string&, std::vector<BYTE>&);
			};
		}
	}
}

#endif

