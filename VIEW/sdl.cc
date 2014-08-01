#include <map>
#include <SDL2/SDL.h>
#include "sdl.h"

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

	void start(const char *wn, int w, int h)
	{
    	SDL_Init(SDL_INIT_EVERYTHING);

		SDL_Window *win(
			SDL_CreateWindow(wn, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_OPENGL));
		
		SDL_GL_CreateContext(win);

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

		controls[SDLK_w] = Controls::UP;
		controls[SDLK_a] = Controls::LEFT;
		controls[SDLK_d] = Controls::RIGHT;
		controls[SDLK_s] = Controls::DOWN;
		controls[SDLK_q] = Controls::ASCEND;
		controls[SDLK_e] = Controls::DESCEND;
		controls[SDLK_ESCAPE] = Controls::ESCAPE;
		controls[SDLK_SPACE] = Controls::SPACE;
		controls[SDLK_LSHIFT] = Controls::SHIFT;
		controls[SDLK_RSHIFT] = Controls::SHIFT;
		controls[SDLK_RETURN] = Controls::RETURN;
//		controls[SDLK_q] = Controls::Q;
//		controls[SDLK_l] = Controls::A;
//		controls[SDLK_p] = Controls::B;
//		controls[SDLK_k] = Controls::X;
//		controls[SDLK_o] = Controls::Y;
//		controls[SDLK_c] = Controls::L;
//		controls[SDLK_m] = Controls::R;
//		controls[SDLK_RETURN] = Controls::START;
//		controls[SDLK_BACKSPACE] = Controls::SELECT;
	}
}

