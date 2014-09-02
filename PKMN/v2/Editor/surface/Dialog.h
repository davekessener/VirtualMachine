#ifndef PKMN_EDITOR_SURFACE_DIALOG_H
#define PKMN_EDITOR_SURFACE_DIALOG_H

#include "../../common.h"
#include <string>
#include <initializer_list>
#include <functional>
#include "Surface.h"

namespace editor
{
	namespace surface
	{
		class Dialog : public Surface
		{
			typedef std::function<void(void)> v_fn;
			typedef std::pair<std::string, v_fn> btn_t;
			using Surface::MouseButtons;

			public:
				Dialog(const std::string&, std::initializer_list<btn_t> = std::initializer_list<btn_t>());
			private:
				void i_doInit( );
				void i_doUpdate(int);
				void i_doRender( ) const;
			private:
				std::string msg_;
				std::vector<Surface_ptr> btns_;
		};
	}
}

#endif

