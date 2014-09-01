#ifndef PKMN_EDITOR_SURFACE_DIALOG_H
#define PKMN_EDITOR_SURFACE_DIALOG_H

#include "../../common.h"
#include <string>
#include "Surface.h"

namespace editor
{
	namespace surface
	{
		class Dialog : public Surface
		{
			using Surface::MouseButtons;

			public:
				Dialog(const std::string& msg) : msg_(msg) { }
			private:
				void i_doInit( );
				void i_doUpdate(int);
				void i_doRender( ) const;
			private:
				std::string msg_;
		};
	}
}

#endif

