#ifndef MAP_EDITOR_H
#define MAP_EDITOR_H

#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <map>
#include <vector>
#include <functional>
#include <cstdint>
#include <Misc/SDLException.h>
#include "TilesetWindow.h"

class Editor
{
	typedef std::vector<std::string> cmd_params_t;

	public:
		static int run(int, char **);
	private:
		Editor( );
		Editor(const Editor&);
		~Editor( );
		Editor& operator=(const Editor&);
		static cmd_params_t parse(const std::string&);

		void execute(const std::string&);
		void save(const std::string&);

		bool running;
		TilesetWindow tsWin;
		std::map<std::string, std::function<void(cmd_params_t)>> cmds;
};

#endif

