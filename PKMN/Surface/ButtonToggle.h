#ifndef SURFACE_BUTTONTOGGLE_H
#define SURFACE_BUTTONTOGGLE_H

#include "Button.h"

namespace surface
{
	class ButtonToggle : public Button
	{
		public:
			ButtonToggle(Image *i, int x, int y, onAction_fn fn, const std::string& s1, const std::string& s2 = "")
				: Button(i, x, y, fn, s1, s2) { }
			void mouseDown(button_t, int, int);
			void mouseDrag(button_t, int, int);
			void mouseUp(button_t, int, int);
		private:
	};
}

#endif

