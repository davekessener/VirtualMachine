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
					static DWORD LoadPNG(const std::string&);
					static int LoadRawPNG(const std::string&, std::vector<BYTE>&);
			};
		}
	}
}

#endif

