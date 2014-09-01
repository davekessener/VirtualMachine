#ifndef PKMN_EDITOR_SURFACE_MENU_H
#define PKMN_EDITOR_SURFACE_MENU_H

#include "../../common.h"
#include <map>
#include <functional>
#include "Surface.h"

namespace editor
{
	namespace surface
	{
		class Menu : public Surface
		{
			public:
			typedef std::function<void(void)> menu_fn;
			typedef std::vector<std::pair<std::string, menu_fn>> item_map;
			typedef std::vector<std::pair<std::string, item_map>> menu_map;
			typedef std::vector<Surface_ptr> menu_vec;
			using Surface::MouseButtons;

			public:
				void setContent(Surface_ptr);
				void addItem(const std::string&, const std::string&, menu_fn);
			private:
				void i_doInit( );
				void i_doUpdate(int);
				void i_doRender( ) const;
				void i_doMouseUp(MouseButtons, int, int);
			private:
				menu_map menudata_;
				menu_vec menus_;
				Surface_ptr curMenu_;
		};
	}
}

#endif

