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
			typedef std::function<void(void)> sig_fn;
			using Surface::MouseButtons;
			using Surface::Controls;

			public:
				StringInput(sig_fn sig = nullptr) : sig_(sig) { }
				inline void setInput(const std::string& s) { str_ = s; idx_ = o_ = 0; dirty(); }
				inline const std::string& getInput( ) const { return str_; }
			private:
				void i_doInit( );
				void i_doUpdate(int);
				void i_doRender( ) const;
				void i_doMouseUp(MouseButtons, int, int);
				void i_doKeyDown(Controls);
			private:
				sig_fn sig_;
				std::string str_;
				int idx_, o_, ml_;
		};
	}
}

#endif

