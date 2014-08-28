#ifndef SURFACE_BUTTONTOGGLE_H
#define SURFACE_BUTTONTOGGLE_H

#include "Button.h"

namespace surface
{
	class ButtonToggle : public Button
	{
		public:
			ButtonToggle(Image *, int, int, onAction_fn, const std::string&, const std::string& = "");
			void mouseDown(button_t, int, int);
			void mouseDrag(button_t, int, int);
			void mouseUp(button_t, int, int);
		private:
	};
}

#endif

