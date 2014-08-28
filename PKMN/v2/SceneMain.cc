#include "SceneMain.h"
#include "Config.h"
#include <dav/Logger.h>

namespace pkmn
{
	SceneMain::SceneMain(const params_t& args)
	{
		Config::loadConfig("config.ini");
	}

	void SceneMain::init(void)
	{
	}

	void SceneMain::update(int d)
	{
	}

	void SceneMain::input(Controls key, bool pressed)
	{
		if(pressed && key == Controls::Q) quit();
	}

	void SceneMain::render(void) const
	{
	}

	void SceneMain::release(void)
	{
	}
}

