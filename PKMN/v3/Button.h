#ifndef DAV_PKMN_V3_SCREEN_BUTTON
#define DAV_PKMN_V3_SCREEN_BUTTON

#include "Surface.h"
#include "Icon.h"

namespace dav
{
	namespace pkmn
	{
		namespace screen
		{
			class Button : public Surface
			{
				public:
					void loadIcon(const std::string&);
				private:
			};
		}
	}
}

#endif

