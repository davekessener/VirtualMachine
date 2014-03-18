#include "Settings.h"

const std::string editor::Settings::TS_GRID("show_tileset_grid");
const std::string editor::Settings::MAP_GRID("show_map_grid");
const std::string editor::Settings::TS_BUTTON_GRID("grid_btn");
const std::string editor::Settings::NBT_MAP_NAME("nbt_map_name");
const std::string editor::Settings::NBT_MAP_ID("nbt_map_id");
const std::string editor::Settings::NBT_MAP_WIDTH("nbt_map_w");
const std::string editor::Settings::NBT_MAP_HEIGHT("nbt_map_h");
const std::string editor::Settings::NBT_MAP_DATA("nbt_map_data");
const std::string editor::Settings::NBT_MAP_DATA_BOTTOM("nbt_map_data_b");
const std::string editor::Settings::NBT_MAP_DATA_INTER("nbt_map_data_i");
const std::string editor::Settings::NBT_MAP_DATA_ANIMATION("nbt_map_data_a");
const std::string editor::Settings::NBT_MAP_DATA_TOP("nbt_map_data_t");

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

