#ifndef PKMN_EDITOR_SURFACE_BUTTONHELPER_H
#define PKMN_EDITOR_SURFACE_BUTTONHELPER_H

#include "../../common.h"

namespace editor
{
	namespace surface
	{
		class ButtonHelper
		{
			public:
				static void renderButton(int, int, int, int, bool = false);
			private:
				ButtonHelper( );
				static ButtonHelper& instance( ) { static ButtonHelper bh; return bh; }
			private:
				DWORD id1_, id2_;
		};
	}
}

#endif

