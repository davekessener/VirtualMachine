#ifndef RENDERMANAGER_H
#define RENDERMANAGER_H

#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <thread>
#include <map>
#include <vector>
#include <cstdint>
#include <Misc/Timer.h>

class RenderManager
{
	public:
		static bool poll(std::uint32_t id, SDL_Event *e) { return instance()._poll(id, e); }
	private:
		RenderManager( ) : running(true), thread(&RenderManager::run, this) { }
		~RenderManager( ) { running = false; thread.join(); }
		static RenderManager& instance( );
		bool _poll(std::uint32_t, SDL_Event *);
		void run( );

		bool running;
		std::thread thread;
		std::map<std::uint32_t, std::vector<SDL_Event>> evns;
};

#endif

