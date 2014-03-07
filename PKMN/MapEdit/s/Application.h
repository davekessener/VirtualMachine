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

		{
		}
	}
}

#endif

