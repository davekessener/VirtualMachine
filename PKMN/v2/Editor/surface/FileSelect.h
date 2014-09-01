#ifndef PKMN_EDITOR_SURFACE_FILESELECT_H
#define PKMN_EDITOR_SURFACE_FILESELECT_H

#include "../../common.h"
#include "Surface.h"

namespace editor
{
	namespace surface
	{
		class FileSelect : public Surface
		{
			typedef std::vector<std::string> svec_t;
			using Surface::Controls;
			using Surface::MouseButtons;

			public:
			private:
				void i_doInit( );
				void i_doUpdate(int);
				void i_doPrerender( );
				void i_doRender( ) const;
				void i_doMouseUp(MouseButtons, int, int);
				void i_doKeyDown(Controls);
			private:
				std::string dir_, cur_;
				int idx_;
				Surface_ptr list_;
		};
	}
}

#endif

