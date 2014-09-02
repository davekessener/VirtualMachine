#ifndef PKMN_EDITOR_SURFACE_VIEWER_H
#define PKMN_EDITOR_SURFACE_VIEWER_H

#include "../../common.h"
#include "Surface.h"

namespace editor
{
	namespace surface
	{
		class Viewer : public Surface
		{
			public:
			private:
				void i_doInit( );
				void i_doUpdate(int);
				void i_doPrerender( );
				void i_doRender( ) const;
			private:
		};
	}
}

#endif

