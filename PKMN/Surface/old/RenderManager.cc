#include "RenderManager.h"

RenderManager& RenderManager::instance(void)
{
	static RenderManager rm;
	return rm;
}

bool RenderManager::_poll(std::uint32_t id, SDL_Event *e)
{
	if(e && evns.count(id) > 0)
	{
		std::vector<SDL_Event> &v = evns.at(id);

		if(!v.empty())
		{
			*e = v.at(0);
			v.erase(v.begin());
		}
	}
}

void RenderManager::run(void)
{
	Timer t;
	SDL_Event e;

	while(running)
	{
		if(SDL_PollEvent(&e))
		{
			if(e.type == SDL_WINDOWEVENT)
			{
				evns[e.window.windowID].push_back(e);
			}
		}

		t.sleep(5);
	}
}

