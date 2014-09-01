#ifndef PKMN_EDITOR_SURFACE_STRINGINPUT_H
#define PKMN_EDITOR_SURFACE_STRINGINPUT_H

#include "../../common.h"
#include "Surface.h"
#include <functional>

namespace editor
{
	namespace surface
	{
		class StringInput : public Surface
		{
			typedef std::function<void(const std::string&)> input_fn;
			using Surface::MouseButtons;
			using Surface::Controls;

			public:
				StringInput(input_fn in) : in_(in) { }
			private:
				void i_doInit( );
				void i_doUpdate(int);
				void i_doRender( ) const;
				void i_doMouseUp(MouseButtons, int, int);
				void i_doKeyDown(Controls);
			private:
				input_fn in_;
				std::string str_;
				int idx_, o_;
		};
	}
}

#endif

