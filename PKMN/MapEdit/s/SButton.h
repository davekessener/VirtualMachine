#ifndef _SBUTTON_H
#define _SBUTTON_H

#include <functional>
#include "Surface.h"
#include "WindowHandler.h"

#ifdef _SBUTTON_MAIN
#define __SB_STD_1 "_1"
#define __SB_STD_2 "_2"
#endif

class SButton : public Surface
{
	public:
	typedef std::uint32_t button_state;
	typedef std::function<void(button_state)> onAction_fn;

	public:
		SButton(Image *, int, int, onAction_fn, const std::string&, const std::string& = "");
		~SButton( );
		void mouseDown(button_t, int, int);
		void mouseDrag(button_t, int, int);
		void mouseUp(button_t, int, int);

		static const int NONE = 0;
		static const int PRESSED = 1;
		static const int INACTIVE = 2;
		static const int RELEASED = 3;
	protected:
		void draw( );
	private:
		void changeState(button_state);
		button_state state;
		onAction_fn action;
		Image *buttons[2];
};

#endif

