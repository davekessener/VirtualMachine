#ifndef PKMN_EDITOR_SURFACE_FILESELECT_H
#define PKMN_EDITOR_SURFACE_FILESELECT_H

#include "../../common.h"
#include "Surface.h"
#include <functional>

namespace editor
{
	namespace surface
	{
		class FileSelect : public Surface
		{
			typedef std::vector<std::string> svec_t;
			typedef std::function<void(const std::string&)> select_fn;
			using Surface::Controls;
			using Surface::MouseButtons;

			public:
				FileSelect(select_fn s) : select_(s) { }
			private:
				void i_doInit( );
				void i_doUpdate(int);
				void i_doPrerender( );
				void i_doRender( ) const;
				void i_doMouseUp(MouseButtons, int, int);
				void i_doKeyDown(Controls);
			private:
				select_fn select_;
				std::string dir_, cur_;
				Surface_ptr list_, input_;
		};
	}
}

#endif

