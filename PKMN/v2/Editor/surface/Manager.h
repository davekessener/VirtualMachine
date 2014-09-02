#ifndef PKMN_EDITOR_SURFACE_MANAGER_H
#define PKMN_EDITOR_SURFACE_MANAGER_H

#include "../../common.h"
#include "Surface.h"
#include <functional>

namespace editor
{
	namespace surface
	{
		class Manager : public Surface
		{
			typedef std::function<void(Surface_ptr, float, float)> dialog_fn;

			public:
				Manager(dialog_fn dfn) : dialog_(dfn) { }
			private:
				void i_doInit( );
				void i_doUpdate(int);
				void i_doPrerender( );
				void i_doRender( ) const;
				void updateContent(const std::string&);
			private:
				dialog_fn dialog_;
				Surface_ptr list_, content_, tileset_;
		};
	}
}

#endif

