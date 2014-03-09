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
#include "TilesetSurface.h"

class Editor
{
	typedef std::vector<std::string> cmd_params_t;

	public:
		static int run(int = 0, char ** = NULL);
	private:
		Editor(int, int);
		Editor(const Editor&);
		~Editor( );
		Editor& operator=(const Editor&);
		static cmd_params_t parse(const std::string&);
		static button_t toButton(std::int8_t);

		void execute(const std::string&);
		void save(const std::string&);
		void handle(SDL_Event&);
		void render( );
		void onMouseDown(SDL_MouseButtonEvent);
		void onMouseUp(SDL_MouseButtonEvent);
		void onMouseDrag(SDL_MouseMotionEvent);

		bool running;
		int width, height;
		std::map<std::string, std::function<void(cmd_params_t)>> cmds;
		SDL_Window *win;
		SDL_Renderer *renderer;
		TilesetSurface *tileset;
		Surface *inputSurface;

		static const int FRAME_RATE = 60;
};

#endif

