#ifndef DAV_PKMN_V3_SCREEN_BUTTON
#define DAV_PKMN_V3_SCREEN_BUTTON

#include "Surface.h"
#include "Icon.h"

namespace dav
{
	namespace pkmn
	{
		namespace screen
		{
			class Button : public Surface
			{
				using Surface::MouseButtons;

				public:
				typedef std::shared_ptr<Button> Button_ptr;

				public:
					void loadIcon(const std::string&);
					void loadText(const std::string&);
				private:
					void removeContent( );
					void press(bool);
				private:
					void i_doInit( );
					void i_doRender( );
					void i_doMouseDown(MouseButtons, int, int);
					void i_doMouseUp(MouseButtons, int, int);
					void i_doMouseDrag(int, int);
					Surface_ptr i_doGetControlAt(int, int);
				private:
					text_info up_, down_;
					bool pressed_, active_;
					Surface_ptr content_;
			};

			typedef Button::Button_ptr Button_ptr;
		}
	}
}

#endif

