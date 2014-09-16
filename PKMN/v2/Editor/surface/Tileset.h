#ifndef PKMN_EDITOR_SURFACE_TILESET_H
#define PKMN_EDITOR_SURFACE_TILESET_H

#include "../../common.h"
#include "Scrollable.h"
#include "../TileBlock.h"

namespace editor
{
	namespace surface
	{
		class Tileset : public Scrollable
		{
			using Surface::MouseButtons;

			public:
			private:
				void i_doInit( );
				void i_doUpdate(int);
				void i_doRender( ) const;
				void i_doMouseDown(MouseButtons, int, int);
				void i_doMouseMove(int, int);
				void i_doMouseUp(MouseButtons, int, int);
				point getScrollSizes( ) const;
				void setScrollPos(int, int);
			private:
				TileBlock id_;
				DWORD did_;
				bool down_;
				point o_, s_;
		};
	}
}

#endif

