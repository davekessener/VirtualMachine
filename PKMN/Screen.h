#ifndef SCREEN_H
#define SCREEN_H

#include <SDL2/SDL.h>
#include <iostream>
#include <functional>
#include <string>
#include <deque>
#include <map>
#include <cstdlib>
#include "Terminal.h"

#ifdef SCREEN_MAIN
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <cassert>
#include "SDLException.h"
#endif

namespace Controls
{
	enum
	{
		NONE,
		A,
		B,
		X,
		Y,
		UP,
		DOWN,
		LEFT,
		RIGHT,
		START,
		SELECT,
		L,
		R
	};
}

class Screen : public display::Terminal
{
	typedef std::function<void(void)> quitFn_t;

	public:
		Screen( );
		~Screen( );
		void init( );
		void finalize( );
		void flush( );
		void getCursorPos(int&, int&);
		void setCursorPos(int, int);
		void getScreenSize(int&, int&);
		void showCursor(bool);
		void eraseScreen( );
		void eraseLine( );
		int getChar( );
		void pause( );
		void printf(const char *, va_list);
		void printfw(const wchar_t *, va_list);
	public:
		static Screen& instance( );
		SDL_Renderer *getRenderer( ) { return renderer; }
		void onQuit(quitFn_t);
		SDL_Texture *loadImage(const std::string&);
		void toScreen(SDL_Texture *, SDL_Rect o, SDL_Rect r);
	public:
		static const int SCREEN_WIDTH = 240;
		static const int SCREEN_HEIGHT = 176;
	protected:
	private:
		static void loadKeyMap(const std::string&, std::map<int, int>&);
		void pressKey(int);
		void releaseKey(int);
		SDL_Window *window;
		SDL_Renderer *renderer;
		std::deque<int> keyPressed;
		std::map<int, int> keyMap;
		quitFn_t quitFn;
};

#endif

