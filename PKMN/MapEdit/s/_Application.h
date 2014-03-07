#ifndef _APPLICATION_H
#define _APPLICATION_H

#include "Surface.h"
#include "Logger.h"

namespace surface
{
	template<typename T>
	class Application : public T
	{
		public:
			Application(int = SDL_WINDOWPOS_CENTERED, int = SDL_WINDOWPOS_CENTERED, int = 0, int = 0);
			~Application( );
			void run( );
		protected:
			void handle(SDL_Event&);
			void execute(const std::string&);
		private:
			bool running;

			static Image *init(int, int, int, int);
			static void end( );
			static Image *image;
			static SDL_Window *win;
			static SDL_Renderer *render;

			static const int FRAME_RATE = 60;
	};

	template<typename T>
	Application<T>::Application(int x, int y, int w, int h) : T(init(x, y, w, h)), running(false)
	{
	}

	template<typename T>
	Application<T>::~Application(void)
	{
		end();
	}

	template<typename T>
	void Application<T>::run(void)
	{
		running = true;

		try
		{
			AsyncIn in;
			Timer t;
			SDL_Event e;
			std::string command;

			std::cout << "> " << std::flush;

			while(running)
			{
				while(SDL_PollEvent(&e))
				{
					handle(e);
				}

				while(in.pollCommand(command))
				{
					execute(command);
					if(running) std::cout << "> " << std::flush;
				}

				redraw();

				SDL_RenderClear(renderer);
				SDL_RenderCopy(renderer, static_cast<SDL_Texture *>(*image), NULL, NULL);
				SDL_RenderPresent(renderer);

				if(running) t.keepRate(FRAME_RATE);
			}
		}
		catch(const Exception& e)
		{
			std::cerr << "ERR: " << e.what() << std::endl;
		}

		running = false;
	}

	template<typename T>
	void Application<T>::handle(SDL_Event& e)
	{
		static Surface *locked = NULL;

		switch(e.type)
		{
			case SDL_QUIT:
				running = false;
				break;
			case SDL_MOUSEBUTTONDOWN:
				locked = lock(e.button.x, e.button.y);
				locked->mouseDown(SDL_BUTTON(e.button.button), e.button.x, e.button.y);
				break;
			case SDL_MOUSEMOTION:
				(locked ? locked : lock(e.motion.x, e.motion.y))->mouseDrag(e.motion.state, e.motion.x, e.motion.y);
				break;
			case SDL_MOUSEBUTTONUP:
				if(locked)
				{
					locked->mouseUp(SDL_BUTTON(e.button.button), e.button.x, e.button.y);
					locked = NULL;
				}
				break;
		}
	}

	template<typename T>
	void Application<T>::execute(const std::string& cmd)
	{
		if(cmd.compare("quit") == 0) running = false;
	}

	template<typename T>
	Image *Application<T>::init(int x, int y, int w, int h)
	{
		if(SDL_Init(SDL_INIT_EVERYTHING)) throw SDLException();
		if(IMG_Init(IMG_INIT_PNG) == -1) throw SDLException(IMG_GetError());

		SDL_DisplayMode dMode;

		if(SDL_GetCurrentDisplayMode(0, &dMode)) throw SDLException();

		if(w < 0) w = dMode.w;
		if(h < 0) h = dMode.h;

		LOG("Set size to %d x %d", w, h);

		if(!(win = SDL_CreateWindow("", x, y, w, h, SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS))) throw SDLException();
		if(!(renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_TARGETTEXTURE))) throw SDLException();
		if(SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND)) throw SDLException();

		Image::init(renderer);

		return image = new Image(w, h);
	}

	template<typename T>
	void Application<T>::end(void)
	{
		delete image;

		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(win);

		Image::init(NULL);

		IMG_Quit();
		SDL_Quit();
	}
}

#endif

