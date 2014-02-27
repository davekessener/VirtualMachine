#ifndef SCREEN_H
#define SCREEN_H

#include <iostream>
#include <SDL2/SDL.h>
#include "Terminal.h"

#ifdef SCREEN_MAIN
#include <SDL_stdinc.h>
#include <cassert>
#endif

class Screen : public display::Terminal
{
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
		bool shouldQuit( );
		SDL_Surface *loadOptimizedImage(const std::string&);
		void toScreen(SDL_Surface *, SDL_Rect o, SDL_Rect r);
	public:
		static const int SCREEN_WIDTH = 240 * 3;
		static const int SCREEN_HEIGHT = 160 * 3;
		static const int BPP = 32;
	protected:
	private:
		SDL_Surface *screen;
		SDL_Window *window;
};

#endif

