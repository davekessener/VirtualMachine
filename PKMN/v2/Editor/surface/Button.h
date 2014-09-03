#ifndef PKMN_EDITOR_SURFACE_BUTTON_H
#define PKMN_EDITOR_SURFACE_BUTTON_H

#include "../../common.h"
#include "Surface.h"
#include <functional>

namespace editor
{
	namespace surface
	{
		class Button : public Surface
		{
			typedef std::function<void(void)> enter_fn;
			using Surface::MouseButtons;

			public:
				Button(const std::string& msg, enter_fn fn) : msg_(msg), fn_(fn), down_(false), alt_(false) { }
			private:
				void i_doInit( ) { }
				void i_doUpdate(int) { }
				void i_doRender( ) const;
				void i_doMouseDown(MouseButtons, int, int);
				void i_doMouseMove(int, int);
				void i_doMouseUp(MouseButtons, int, int);
				inline void changeAlt(bool f) { if(f != alt_) { alt_ = f; dirty(); } }
			private:
				std::string msg_;
				enter_fn fn_;
				bool down_, alt_;
		};
	}
}

#endif

