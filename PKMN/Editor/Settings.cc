#include "Settings.h"

const std::string editor::Settings::TS_GRID("show_tileset_grid");
const std::string editor::Settings::MAP_GRID("show_map_grid");
const std::string editor::Settings::MAP_BLUR("show_map_blur");
const std::string editor::Settings::TS_BUTTON_GRID("grid_btn");
const std::string editor::Settings::MAP_BUTTON_BLUR("blur_btn");
const std::string editor::Settings::NBT_MAP_NAME("Name");
const std::string editor::Settings::NBT_MAP_ID("ID");
const std::string editor::Settings::NBT_MAP_WIDTH("Width");
const std::string editor::Settings::NBT_MAP_HEIGHT("Height");
const std::string editor::Settings::NBT_MAP_DATA("Data");
const std::string editor::Settings::NBT_MAP_DATA_BOTTOM("Bottom");
const std::string editor::Settings::NBT_MAP_DATA_INTER("Intermediate");
const std::string editor::Settings::NBT_MAP_DATA_ANIMATION("Animation");
const std::string editor::Settings::NBT_MAP_DATA_TOP("Top");
const std::string editor::Settings::NBT_MAPS("Maps");
const std::string editor::Settings::TS_FN("tileset.png");

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

	Image *Settings::getTileset(void)
	{
		if(!instance()._ts)
		{
			instance()._ts = new Image(TS_FN);
		}

		return instance()._ts;
	}

	Settings::Settings(void) : _ts(NULL)
	{
	}

	Settings::~Settings(void)
	{
	}
}

