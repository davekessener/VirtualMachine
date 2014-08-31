#include "SceneOW.h"
#include <set>
#include "Controller.h"
#include "Keypad.h"
#include <dav/Logger.h>

namespace pkmn
{
	void SceneOW::init(void)
	{
	}

	void SceneOW::update(int d)
	{
		Controller::instance().update(d);
	}

	void SceneOW::render(void) const
	{
		Controller::instance().render();
	}

	void SceneOW::input(Controls key, bool pressed)
	{
		if(pressed && key == Controls::ESCAPE) quit();

		Keypad::press(key, pressed);
	}

	void SceneOW::release(void)
	{
	}
}

