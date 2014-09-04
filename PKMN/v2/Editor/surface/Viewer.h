#ifndef PKMN_EDITOR_SURFACE_VIEWER_H
#define PKMN_EDITOR_SURFACE_VIEWER_H

#include "../../common.h"
#include "Scrollable.h"
#include <cassert>

namespace editor
{
	namespace surface
	{
		class Viewer : public Scrollable
		{
			using Surface::MouseButtons;

			public:
			private:
				void i_doInit( );
				void i_doUpdate(int);
				void i_doPrerender( );
				void i_doRender( ) const;
				void i_doMouseDown(MouseButtons, int, int);
				void i_doMouseMove(int, int);
				void i_doMouseUp(MouseButtons, int, int);
				point getScrollSizes( ) const;
				void setScrollPos(int, int);
				void setLayer(int);
				template<typename T>
					inline std::shared_ptr<T> getButton(uint i)
						{ assert(i<4); return std::dynamic_pointer_cast<T>(btns_[i]); }
			private:
				int layer_;
				bool down_;
				point s_, o_;
				Surface_ptr btns_[4];
		};
	}
}

#endif

