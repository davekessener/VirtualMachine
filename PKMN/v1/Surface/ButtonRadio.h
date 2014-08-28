#ifndef SURFACE_BUTTONRADIO_H
#define SURFACE_BUTTONRADIO_H

#include <initializer_list>
#include "ButtonToggle.h"

namespace surface
{
	class ButtonRadio : public ButtonToggle
	{
		public:
			ButtonRadio(Image *, int, int, onAction_fn, const std::string&, const std::string& = "");
			void mouseDown(button_t, int, int);
			void connect(ButtonRadio&);
		protected:
		private:
			std::vector<ButtonRadio *> group_;
	};

	class RadioButtonGroup
	{
		public:
			RadioButtonGroup& add(ButtonRadio *);
			void join( );
		private:
			std::vector<ButtonRadio *> btns_;
	};
}

#endif

