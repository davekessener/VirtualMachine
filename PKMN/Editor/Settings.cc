#include "Settings.h"

const std::string editor::Settings::TS_GRID("show_tileset_grid");
const std::string editor::Settings::TS_BUTTON_GRID("grid_btn");

namespace editor
{
	bool Settings::getBool(const std::string& key)
	{
		std::map<std::string, bool> &map = instance().boolMap;

		return map.count(key) > 0 ? map.at(key) : false;
	}

	void Settings::setBool(const std::string& key, bool val)
	{
		std::map<std::string, bool> &map = instance().boolMap;

		map[key] = val;
	}
}

