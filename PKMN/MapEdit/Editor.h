#ifndef MAP_EDITOR_H
#define MAP_EDITOR_H

#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <map>
#include <vector>
#include <cstdint>
#include <Misc/SDLException.h>
#include <Misc/Timer.h>
#include <Misc/Logger.h>
#include "AsyncIn.h"
#include "Window.h"
#include "TilesetWindow.h"

class Editor
{
	typedef std::vector<std::string> cmd_params_t;

	public:
		static int run(int = 0, char ** = NULL);
	private:
		Editor( );
		Editor(const Editor&);
		~Editor( );
		Editor& operator=(const Editor&);
		static cmd_params_t parse(const std::string&);

		void execute(const std::string&);
		void save(const std::string&);
		void handle(SDL_Event&);
		void render( );

		bool running;
		std::map<std::string, std::function<void(cmd_params_t)>> cmds;
		std::map<win_id_t, Window *> windows;
		TilesetWindow tileset;

		static const int FRAME_RATE = 60;
};

#endif

