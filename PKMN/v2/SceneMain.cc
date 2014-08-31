#include "SceneMain.h"
#include "Config.h"
#include <dav/Logger.h>
#include "Controller.h"
#include "SceneOW.h"

namespace pkmn
{
	SceneMain::SceneMain(const params_t& args)
	{
		Config::loadConfig("config.ini");
	}

	void SceneMain::init(void)
	{
		if(!Controller::load("save.nbt"))
		{
			Controller::generate("DAVE");
		}

		change(Scene_ptr(new SceneOW));
	}

	void SceneMain::update(int d)
	{
	}

	void SceneMain::input(Controls key, bool pressed)
	{
	}

	void SceneMain::render(void) const
	{
	}

	void SceneMain::release(void)
	{
	}
}

