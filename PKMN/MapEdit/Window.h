#ifndef MAP_WINDOW_H
#define MAP_WINDOW_H

#include <string>
#include <sstream>
#include <cstdint>
#include <SDL2/SDL.h>
#include <Misc/SDLException.h>
#include <Misc/Logger.h>

typedef std::uint32_t win_id_t;

typedef std::uint32_t button_t;
namespace Button
{
	enum
	{
		LEFT = 0,
		RIGHT = 1,
		SHIFT = 2,
		CTRL = 4,
		ALT = 8,
		WIN = 16
	};
}

class Window
{
	public:
		Window( );
		virtual ~Window( );
		void init( );
		win_id_t getID( ) const { return SDL_GetWindowID(win); }
		void setTitle(const std::string&);
		const std::string& getTitle( ) const { return title; }
		void refresh( );
		virtual void onClick(button_t, int, int) = 0;
	protected:
		virtual SDL_Window *createWindow( ) = 0;
		virtual void draw( ) = 0;
		SDL_Renderer *getRenderer( ) { return render; }
	private:
		SDL_Window *win;
		SDL_Renderer *render;
		std::string title;
};

#endif

