#ifndef _STATUSBAR_H
#define _STATUSBAR_H

#include "Button.h"

#ifdef _STATUSBAR_MAIN
#define __SB_CLOSE_BTN "close"
#define __SB_MINIMIZE_BTN "mini"
#endif

namespace surface
{
	class StatusBar : public Surface
	{
		public:
			StatusBar(Image *, const std::string& = "");
			~StatusBar( );
			void setTitle(const std::string&);
		protected:
			void draw( );
		private:
			std::string title;
			Button *close, *minimize;
			Image *bg;

			static const int BORDER_SIZE = 3;
			static const int BAR_SIZE = TILE_SIZE + 4;
			static const unsigned int DARK = 0x0a346aff;
			static const unsigned int LIGHT= 0xa6caf0ff;
	};
}

#endif

