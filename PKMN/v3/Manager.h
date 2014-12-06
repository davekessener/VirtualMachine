#ifndef DAV_PKMN_V3_SURFACE_MANAGER_H
#define DAV_PKMN_V3_SURFACE_MANAGER_H

#include <string>
#include "common.h"
#include "Surface.h"

namespace dav
{
	namespace pkmn
	{
		namespace screen
		{
			void runProgram(const std::string& s, Surface_ptr);
		}
	}
}

#endif

