#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <string>
#include <vector>
#include "Map.h"
#include "OWSprite.h"
#include "PlayerSprite.h"
#include "Camera.h"
#include <Misc/Logger.h>

#ifdef CONTROLLER_MAIN
#include <NBT/NBT.h>
#include <cassert>
#include "MapRenderer.h"
#include "OWSpriteRenderer.h"
#endif

class Controller
{
	public:
		static void create(const std::string&);
		static void destroy( );
		static Controller& instance( );
		void render( );
		void update( );
		bool input(int);
	private:
		Controller(const std::string&);
		Controller(const Controller&);
		~Controller( );
		Controller& operator=(const Controller&);
		static Controller *_instance;

		PlayerSprite thePlayer;
		Camera camera;
		std::string saveFn;
		std::vector<Map *> maps;
		int curMap, ticks;
};

#endif

