#include "SceneMain.h"
#include "Config.h"
#include <dav/Logger.h>
#include "Map.h"
#include "Player.h"

namespace pkmn
{
	std::shared_ptr<Map> mp;
	int x, y;

	SceneMain::SceneMain(const params_t& args)
	{
		Config::loadConfig("config.ini");
	}

	void SceneMain::init(void)
	{
		Player p("DAVE");
		nbt::writeFile("save_0.nbt", p.save());
		LOG("UHM. SAVE?");
	}

	void SceneMain::update(int d)
	{
	}

	void SceneMain::input(Controls key, bool pressed)
	{
		if(pressed)
		{
			switch(key)
			{
				case Controls::Q:
					quit();
					break;
				case Controls::W:
					--y;
					break;
				case Controls::S:
					++y;
					break;
				case Controls::A:
					--x;
					break;
				case Controls::D:
					++x;
					break;
				default:
					break;
			}
		}
	}

	void SceneMain::render(void) const
	{
		if(!static_cast<bool>(mp))
		{
			mp.reset(new Map(1));
			x = y = 0;
		}

		mp->render(x * 2, y * 2);
	}

	void SceneMain::release(void)
	{
	}
}

