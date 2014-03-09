#ifndef _APPLICATION_H
#define _APPLICATION_H

#include <sstream>
#include "Surface.h"
#include "Logger.h"
#include "AsyncIn.h"
#include "Timer.h"
#include "FontManager.h"
#include <SDL_ttf.h>

namespace surface
{
	template<typename T>
	class Application : public T
	{
		typedef std::vector<std::string> params_t;

		public:
			Application(int = SDL_WINDOWPOS_CENTERED, int = SDL_WINDOWPOS_CENTERED, int = 0, int = 0);
			~Application( );
			void run( );
		protected:
			void handle(SDL_Event&);
			void execute(const std::string&);
		private:
			bool running;
			std::map<std::string, std::function<void(params_t)>> cmds;

			static params_t split(const std::string&);
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
		cmds["quit"] = [this](params_t p) { running = false; };
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

				T::redraw();

				SDL_RenderClear(render);
				SDL_RenderCopy(render, static_cast<SDL_Texture *>(*image), NULL, NULL);
				SDL_RenderPresent(render);
				SDL_UpdateWindowSurface(win);

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
				locked = T::lock(e.button.x, e.button.y);
				locked->mouseDown(SDL_BUTTON(e.button.button), e.button.x, e.button.y);
				break;
			case SDL_MOUSEMOTION:
				(locked ? locked : T::lock(e.motion.x, e.motion.y))->mouseDrag(e.motion.state, e.motion.x, e.motion.y);
				break;
			case SDL_MOUSEBUTTONUP:
				if(locked)
				{
					locked->mouseUp(SDL_BUTTON(e.button.button), e.button.x, e.button.y);
					locked = NULL;
				}
				break;
			case SDL_WINDOWEVENT:
				switch(e.window.event)
				{
				}
				break;
		}
	}

	template<typename T>
	void Application<T>::execute(const std::string& cmd)
	{
		params_t p = split(cmd);

		if(p.empty()) return;

		const std::string &c = p[0];
		if(cmds.count(c) > 0)
		{
			cmds.at(c)(p);
		}
	}

// # ===========================================================================

	template<typename T>
	typename Application<T>::params_t Application<T>::split(const std::string& cmd)
	{
		std::istringstream iss(cmd);
		std::vector<std::string> v;

		while(iss)
		{
			std::string s;
			iss >> s;
			if(!s.empty()) v.push_back(s);
		}

		return v;
	}

	template<typename T>
	Image *Application<T>::init(int x, int y, int w, int h)
	{
		if(SDL_Init(SDL_INIT_EVERYTHING)) throw SDLException();
		if(IMG_Init(IMG_INIT_PNG) == -1) throw SDLException(IMG_GetError());
		if(TTF_Init() == -1) throw SDLException(TTF_GetError());

		SDL_DisplayMode dMode;

		if(SDL_GetCurrentDisplayMode(0, &dMode)) throw SDLException();

		if(w <= 0) w += dMode.w;
		if(h <= 0) h += dMode.h;

		LOG("Set size to %d x %d", w, h);

		if(!(win = SDL_CreateWindow("", x, y, w, h, SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS))) throw SDLException();
		if(!(render = SDL_CreateRenderer(win, -1, SDL_RENDERER_TARGETTEXTURE))) throw SDLException();
		if(SDL_SetRenderDrawBlendMode(render, SDL_BLENDMODE_BLEND)) throw SDLException();

		SDL_SetRenderDrawColor(render, 255, 255, 255, 255);

		Image::init(render);

		return image = new Image(w, h);
	}

	template<typename T>
	void Application<T>::end(void)
	{
		delete image;

		FontManager::end();

		SDL_DestroyRenderer(render);
		SDL_DestroyWindow(win);

		Image::init(NULL);

		TTF_Quit();
		IMG_Quit();
		SDL_Quit();
	}

	template<typename T>
	SDL_Renderer *Application<T>::render = NULL;

	template<typename T>
	SDL_Window *Application<T>::win = NULL;

	template<typename T>
	Image *Application<T>::image = NULL;
}

#endif

