#ifndef VM_SCREEN_H
#define VM_SCREEN_H

#include <functional>
#include <string>
#include <SDL2/SDL.h>
#include <deque>
#include "ddef.h"
#include "Key.h"

namespace vm
{
	namespace sdl 
	{
		class Screen
		{
			public:
			typedef std::function<void(void)> quitFn_t;

			public:
				static Screen& instance( );
				void aquire(const std::string&, size_t, size_t);
				void release( );
				void update( );
				void refresh( );
				void clear( );
				void onQuit(quitFn_t);
				bool keyPressed( ) const;
				Key keyPoll( );
			private:
				SDL_Window *window_;
				SDL_Renderer *renderer_;
				quitFn_t quit_;
				std::deque<Key> buffer_;

			private:
				Screen( );
				~Screen( );
				Screen(const Screen&);
				Screen& operator=(const Screen&);
		};
	}
}

#endif

