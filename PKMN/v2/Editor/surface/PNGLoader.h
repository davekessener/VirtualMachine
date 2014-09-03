#ifndef PKMN_EDITOR_SURFACE_PNGLOADER_H
#define PKMN_EDITOR_SURFACE_PNGLOADER_H

#include "../../common.h"

namespace editor
{
	namespace surface
	{
		class PNGLoader
		{
			public:
				static DWORD loadPNG(const std::string&);
		};
	}
}

#endif

