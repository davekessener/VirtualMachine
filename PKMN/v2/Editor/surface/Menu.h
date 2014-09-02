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
			typedef std::function<bool(void)> enable_fn;
			struct Item { std::string id; menu_fn activate; enable_fn enable; bool enabled; };
			typedef std::vector<Item> item_map;
			typedef std::vector<std::pair<std::string, item_map>> menu_map;
			typedef std::vector<Surface_ptr> menu_vec;
			using Surface::MouseButtons;

			public:
				void setContent(Surface_ptr);
				void addItem(const std::string&, const std::string&, menu_fn, enable_fn = nullptr);
			private:
				void i_doInit( );
				void i_doUpdate(int);
				void i_doRender( ) const;
				void i_doMouseOver(bool);
				void i_doMouseMove(int, int);
				void i_doMouseUp(MouseButtons, int, int);
				int getIdxFromX(int) const;
				inline void removeCurMenu( )
					{ if(curMenu_) { removeChild(curMenu_->ID()); curMenu_.reset(); dirty(); } }
			private:
				menu_map menudata_;
				menu_vec menus_;
				Surface_ptr curMenu_;
				int idx_;
		};
	}
}

#endif

