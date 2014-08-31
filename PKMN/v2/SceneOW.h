#ifndef PKMN_SCENEOW_H
#define PKMN_SCENEOW_H

#include "common.h"
#include "Scene.h"
#include "Map.h"

namespace pkmn
{
	class SceneOW : public Scene
	{
		private:
			void init( );
			void update(int);
			void render( ) const;
			void input(Controls, bool);
			void release( );
	};
}

#endif

