#ifndef VM_SCREEN_H
#define VM_SCREEN_H

#include <functional>
//#include <string>
//#include <SDL2/SDL.h>
#include <vector>
#include <deque>
#include "ddef.h"
#include <dav/sdl.h>
//#include "Key.h"

namespace vm
{
	namespace sdl 
	{
		class Screen
		{
			public:
//			typedef std::function<void(void)> quitFn_t;
			typedef std::function<void(void)> render_fn;
			typedef ::sdl::Controls Key;

			public:
				static Screen& instance( );
				void addRender(const render_fn& r) { renders_.push_back(r); }
//				void aquire(const std::string&, size_t, size_t);
//				void release( );
//				void update( );
				void render( ) const { for(const auto& r : renders_) r(); }
//				void clear( );
//				void onQuit(quitFn_t);
				void pressKey(const Key&);
				bool keyPressed( ) const;
				Key keyPoll( );
			private:
//				SDL_Window *window_;
//				SDL_Renderer *renderer_;
//				quitFn_t quit_;
				std::deque<Key> buffer_;
				std::vector<render_fn> renders_;

			private:
				Screen( );
				~Screen( );
				Screen(const Screen&);
				Screen& operator=(const Screen&);
		};
	}
}

#endif

