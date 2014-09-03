#ifndef PKMN_EDITOR_SURFACE_VIEWER_H
#define PKMN_EDITOR_SURFACE_VIEWER_H

#include "../../common.h"
#include "Scrollable.h"

namespace editor
{
	namespace surface
	{
		class Viewer : public Scrollable
		{
			public:
			private:
				void i_doInit( );
				void i_doUpdate(int);
				void i_doPrerender( );
				void i_doRender( ) const;
				point getScrollSizes( ) const;
				void setScrollPos(int, int);
			private:
		};
	}
}

#endif

