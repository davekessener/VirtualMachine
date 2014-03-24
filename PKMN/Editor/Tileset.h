#ifndef EDITOR_TILESET_H
#define EDITOR_TILESET_H

#include <Surface/Scrollable.h>
#include "Settings.h"

namespace editor
{
	class Tileset : public surface::Scrollable
	{
		public:
			Tileset(Image *, int, int, int, int);
			~Tileset( );
			void mouseDown(button_t, int, int);
			void setBackgroundColor(color_rgba_t c) { bgColor = c; }
			void setBackgroundPlain(bool f) { bgPlain = f; }
		protected:
			void draw(Image *, int, int);
			std::pair<int, int> getScrollRanges( );
		private:
			Image *bg, *grid;
			color_rgba_t bgColor;
			bool bgPlain;
			int select;
	
			void createBg( );
	};
}

#endif

