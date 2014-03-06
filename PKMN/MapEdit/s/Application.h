#ifndef _APPLICATION_H
#define _APPLICATION_H

#include "StatusBar.h"
#include "Timer.h"
#include "AsyncIn.h"
#include "Logger.h"

namespace surface
{
	class Application : public Surface
	{
		public:
			template<typename T>
			static void run(int = SDL_WINDOWPOS_CENTERED, int = SDL_WINDOWPOS_CENTERED, int = -1, int = -1);
		protected:
			void handle(SDL_Event&);
			void execute(const std::string&);
			void draw( ) { }
		private:
			Application(Image *);
			~Application( );
			bool running;
			Surface *content;
			StatusBar *status;
			std::map<std::string, std::function<void(std::vector<std::string>)>> commands;

			static const int FRAME_RATE = 60;
	};

	template<typename T>
	void Application::run(int x, int y, int w, int h)
	{
		if(SDL_Init(SDL_INIT_EVERYTHING)) throw SDLException();
		if(IMG_Init(IMG_INIT_PNG) == -1) throw SDLException(IMG_GetError());

		SDL_DisplayMode dMode;

		if(SDL_GetCurrentDisplayMode(0, &dMode)) throw SDLException();

		if(w < 0) w = dMode.w;
		if(h < 0) h = dMode.h;

		LOG("Set size to %d x %d", w, h);

		SDL_Window *win = NULL;
		SDL_Renderer *renderer = NULL;

		if(!(win = SDL_CreateWindow("", x, y, w, h, SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS))) throw SDLException();
		if(!(renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_TARGETTEXTURE))) throw SDLException();
		if(SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND)) throw SDLException();

		Image::init(renderer);

		{
			Image *image = new Image(w, h);
			Application a(image);
			AsyncIn in;

			a.registerSurface(a.content = new T(new SubImage(image, 3, 26, w - 6, h - 29)));

			try
			{
				Timer t;
				SDL_Event e;
				std::string command;

				std::cout << "> " << std::flush;

				while(a.running)
				{
					while(SDL_PollEvent(&e))
					{
						a.handle(e);
					}

					while(in.pollCommand(command))
					{
						a.execute(command);
						if(a.running) std::cout << "> " << std::flush;
					}

					a.redraw();

					SDL_RenderClear(renderer);
					SDL_RenderCopy(renderer, static_cast<SDL_Texture *>(*image), NULL, NULL);
					SDL_RenderPresent(renderer);

					if(a.running) t.keepRate(FRAME_RATE);
				}
			}
			catch(const Exception& e)
			{
				std::cerr << "ERR: " << e.what() << std::endl;
			}
		}

		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(win);

		Image::init(NULL);

		IMG_Quit();
		SDL_Quit();
	}
}

#endif

