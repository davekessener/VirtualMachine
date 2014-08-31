#ifndef PKMN_SCENEMAIN_H
#define PKMN_SCENEMAIN_H

#include "common.h"
#include "Scene.h"

namespace pkmn
{
	class SceneMain : public Scene
	{
		using Scene::Controls;

		public:
			SceneMain(const params_t&);
		private:
			void init( );
			void update(int);
			void input(Controls, bool);
			void render( ) const;
			void release( );
	};
}

#endif

