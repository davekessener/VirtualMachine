#ifndef _BUTTON_H
#define _BUTTON_H

#include <functional>
#include "Surface.h"
#include "WindowHandler.h"
#include "Logger.h"

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
			~Button( );
			void mouseDown(button_t, int, int);
			void mouseDrag(button_t, int, int);
			void mouseUp(button_t, int, int);
	
//			static const int NONE = 0;
//			static const int PRESSED = 1;
//			static const int INACTIVE = 2;
//			static const int RELEASED = 3;
			enum
			{
				NONE,
				PRESSED,
				INACTIVE,
				RELEASED
			};
		protected:
			void draw( );
		private:
			void changeState(button_state);
			button_state state;
			onAction_fn action;
			Image *buttons[2];
	};
}

#endif

