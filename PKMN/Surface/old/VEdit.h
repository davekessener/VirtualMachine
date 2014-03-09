#ifndef MAP_VEDIT_H
#define MAP_VEDIT_H

#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <Misc/SDLException.h>
#include <Misc/Timer.h>
#include <thread>
#include <mutex>
#include <string>
#include "RenderManager.h"

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

class VEdit
{
	public:
		VEdit(const std::string&);
		virtual ~VEdit( );
		void start( );
		void join( );
		const std::string& getTitle( ) const { return title; }
		static const int FRAME_RATE = 60;
	protected:
		virtual void onClick(button_t, int, int) = 0;
		virtual void redraw( ) = 0;
		virtual SDL_Window *createWindow( ) = 0;
		SDL_Renderer *getRenderer( ) { return render; }
		void init( );
	private:
		void run( );

		SDL_Window *win;
		SDL_Renderer *render;
		std::thread *thread;
		std::string title;
		bool running;
		std::uint32_t winID;

		static std::mutex c_mutex;
};

#endif

