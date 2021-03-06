#ifndef _BUTTON_H
#define _BUTTON_H

#include <functional>
#include "Surface.h"
#include "WindowHandler.h"
#include "../Misc/Logger.h"

#ifdef _BUTTON_MAIN
#define __B_STD_1 "_1"
#define __B_STD_2 "_2"
#endif

namespace surface
{
	class Button : public Surface
	{
		public:
		typedef std::uint32_t button_state;
		typedef std::function<void(button_state)> onAction_fn;
	
		public:
			Button(Image *, int, int, onAction_fn, const std::string&, const std::string& = "");
			virtual ~Button( );
			virtual void mouseDown(button_t, int, int);
			virtual void mouseDrag(button_t, int, int);
			virtual void mouseUp(button_t, int, int);
			virtual void changeState(button_state);
	
			enum
			{
				NONE,
				PRESSED,
				INACTIVE,
				RELEASED
			};
		protected:
			void draw(Image *);
			button_state getState( ) { return state; }
		private:
			button_state state;
			onAction_fn action;
			Image *buttons[2];
	};
}

#endif

