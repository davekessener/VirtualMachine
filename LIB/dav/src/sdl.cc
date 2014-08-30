#include <map>
#include <SDL2/SDL.h>
#include "sdl.h"

namespace dav
{
	namespace sdl
	{
		init_t init_fn = nullptr;
		update_t update_fn = nullptr;
		kb_input_t kb_input_fn = nullptr;
		m_input_t m_input_fn = nullptr;
	
		std::map<int, Controls> controls;
		void init_controls( );
	
		void trap_mouse(bool trapped)
		{
			SDL_SetRelativeMouseMode(trapped ? SDL_TRUE : SDL_FALSE);
		}
	
		void start(const char *wn, int w, int h, bool full)
		{
	    	SDL_Init(SDL_INIT_EVERYTHING);
	
			SDL_Window *win(
				SDL_CreateWindow(wn, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, 
					(full ? SDL_WINDOW_FULLSCREEN : 0) | SDL_WINDOW_OPENGL));
			
			SDL_GL_CreateContext(win);

			SDL_GL_SetSwapInterval(1);
	
			bool running = true;
	    	Uint32 start, delta(60);
	    	SDL_Event e;
	
			init_controls();
	
			static const unsigned int FRAME_RATE = 1000 / 60; // 60fps
	
	    	if(init_fn) init_fn(w, h);
	    	
			while(running)
			{
	    	    start = SDL_GetTicks();
	
	    	    if(update_fn) running = update_fn(delta);
	
	    	    while(SDL_PollEvent(&e))
				{
	    	        switch(e.type)
					{
	    	            case SDL_QUIT:
	    	                running = false;
	    	                break;
						case SDL_KEYDOWN:
							if(kb_input_fn)
							{
								auto i = controls.find(e.key.keysym.sym);
								if(i != controls.end())
								{
									kb_input_fn(i->second, true);
								}
							}
							break;
						case SDL_KEYUP:
							if(kb_input_fn)
							{
								auto i = controls.find(e.key.keysym.sym);
								if(i != controls.end())
								{
									kb_input_fn(i->second, false);
								}
							}
							break;
						case SDL_MOUSEMOTION:
							if(m_input_fn)
							{
								m_input_fn(e.motion.x, e.motion.y, e.motion.xrel, e.motion.yrel);
							}
							break;
	    	        }
	    	    }
	
				SDL_GL_SwapWindow(win);
	    	    
				delta = SDL_GetTicks() - start;
				if(FRAME_RATE > delta)
				{
	    	        SDL_Delay(FRAME_RATE - delta);
				}
	    	}
	
			SDL_SetRelativeMouseMode(SDL_FALSE);
	
	    	SDL_Quit();
		}
	
		void set_init(init_t ifn)
		{
			init_fn = ifn;
		}
	
		void set_update(update_t ufn)
		{
			update_fn = ufn;
		}
	
		void set_input(kb_input_t kfn, m_input_t mfn)
		{
			kb_input_fn = kfn;
			m_input_fn = mfn;
		}
	
		void init_controls(void)
		{
			controls.clear();
	
			controls[SDLK_a] = Controls::A;
			controls[SDLK_b] = Controls::B;
			controls[SDLK_c] = Controls::C;
			controls[SDLK_d] = Controls::D;
			controls[SDLK_e] = Controls::E;
			controls[SDLK_f] = Controls::F;
			controls[SDLK_g] = Controls::G;
			controls[SDLK_h] = Controls::H;
			controls[SDLK_i] = Controls::I;
			controls[SDLK_j] = Controls::J;
			controls[SDLK_k] = Controls::K;
			controls[SDLK_l] = Controls::L;
			controls[SDLK_m] = Controls::M;
			controls[SDLK_n] = Controls::N;
			controls[SDLK_o] = Controls::O;
			controls[SDLK_p] = Controls::P;
			controls[SDLK_q] = Controls::Q;
			controls[SDLK_r] = Controls::R;
			controls[SDLK_s] = Controls::S;
			controls[SDLK_t] = Controls::T;
			controls[SDLK_u] = Controls::U;
			controls[SDLK_v] = Controls::V;
			controls[SDLK_w] = Controls::W;
			controls[SDLK_x] = Controls::X;
			controls[SDLK_y] = Controls::Y;
			controls[SDLK_z] = Controls::Z;
			controls[SDLK_0] = Controls::ZERO;
			controls[SDLK_1] = Controls::ONE;
			controls[SDLK_2] = Controls::TWO;
			controls[SDLK_3] = Controls::THREE;
			controls[SDLK_4] = Controls::FOUR;
			controls[SDLK_5] = Controls::FIVE;
			controls[SDLK_6] = Controls::SIX;
			controls[SDLK_7] = Controls::SEVEN;
			controls[SDLK_8] = Controls::EIGHT;
			controls[SDLK_9] = Controls::NINE;
			controls[SDLK_ESCAPE] = Controls::ESCAPE;
			controls[SDLK_SPACE] = Controls::SPACE;
			controls[SDLK_LSHIFT] = Controls::SHIFT;
			controls[SDLK_RSHIFT] = Controls::SHIFT;
			controls[SDLK_RETURN] = Controls::RETURN;
			controls[SDLK_UP] = Controls::UP;
			controls[SDLK_DOWN] = Controls::DOWN;
			controls[SDLK_LEFT] = Controls::LEFT;
			controls[SDLK_RIGHT] = Controls::RIGHT;
			controls[SDLK_LCTRL] = Controls::CTRL;
			controls[SDLK_RCTRL] = Controls::CTRL;
			controls[SDLK_BACKSPACE] = Controls::BACKSPACE;
			controls[SDLK_DELETE] = Controls::DELETE;
			controls[SDLK_TAB] = Controls::TAB;
			controls[SDLK_MINUS] = Controls::DASH;
			controls[SDLK_EQUALS] = Controls::EQUALS;
			controls[SDLK_LEFTBRACKET] = Controls::LSQUARE;
			controls[SDLK_RIGHTBRACKET] = Controls::RSQUARE;
			controls[SDLK_QUOTE] = Controls::QUOTE;
			controls[SDLK_SEMICOLON] = Controls::SEMICOLON;
			controls[SDLK_BACKSLASH] = Controls::BACKSLASH;
			controls[SDLK_COMMA] = Controls::COMMA;
			controls[SDLK_PERIOD] = Controls::PERIOD;
			controls[SDLK_SLASH] = Controls::SLASH;
			controls[SDLK_BACKQUOTE] = Controls::BACKTICK;
		}
	
		inline const char *find(char c, const char *s) { for(; *s ; ++s) if(*s == c) return s; return nullptr; }
	
		char translate(Controls key, bool shift)
		{
			static const char *lo = "0123456789`-=[]'\\,./; ";
			static const char *hi = ")!@#$%^&*(~_+{}\"|<>?: ";
			char c = static_cast<char>(key);
	
			if(c >= 'a' && c <= 'z') return c + (shift ? 'A' - 'a' : 0);
			if(const char *s = find(c, lo)) return *(shift ? hi + (s - lo) : s);
	
			return '\0';
		}
	}
}

