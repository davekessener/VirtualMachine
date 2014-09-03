#ifndef PKMN_EDITOR_SURFACE_TILESET_H
#define PKMN_EDITOR_SURFACE_TILESET_H

#include "../../common.h"
#include "Scrollable.h"

namespace editor
{
	namespace surface
	{
		class Tileset : public Scrollable
		{
			public:
			private:
				void i_doInit( );
				void i_doUpdate(int);
				void i_doRender( ) const;
				point getScrollSizes( ) const;
				void setScrollPos(int, int);
			private:
				point o_, s_;
		};
	}
}

#endif

