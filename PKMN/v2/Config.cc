#include <fstream>
#include "Config.h"
#include <dav/ini.h>
#include <dav/Logger.h>
#include <aux>


#define MXT_TOPIC_GENERAL "General"

#define MXT_GENERAL_DEFPOS "DefaultPosition"
#define MXT_GENERAL_DEFPOS_DEFAULT "0,0"

#define MXT_GENERAL_DEFMAP "DefaultMap"
#define MXT_GENERAL_DEFMAP_DEFAULT 0


#define MXT_TOPIC_RENDER "Render"

#define MXT_RENDER_TILESIZE "TileSize"
#define MXT_RENDER_TILESIZE_DEFAULT 16

#define MXT_RENDER_SCREENWIDTH "ScreenWidth"
#define MXT_RENDER_SCREENWIDTH_DEFAULT 240

#define MXT_RENDER_SCREENHEIGHT "ScreenHeight"
#define MXT_RENDER_SCREENHEIGHT_DEFAULT 160

#define MXT_RENDER_SCREENSCALE "ScreenScale"
#define MXT_RENDER_SCREENSCALE_DEFAULT 1

#define MXT_RENDER_TILESETPATH "TilesetPath"
#define MXT_RENDER_TILESETPATH_DEFAULT "tileset.png"


#define MXT_TOPIC_ABSTRACT "Abstract"

#define MXT_ABSTRACT_MAPPATH "Maps"
#define MXT_ABSTRACT_MAPPATH_DEFAULT "maps.nbt"


#define MXT_NBT_MAPS "Maps"


uint pkmn::Config::TILE_SIZE;
uint pkmn::Config::SCREEN_WIDTH, pkmn::Config::SCREEN_HEIGHT;
uint pkmn::Config::SCREEN_SCALE;
std::string pkmn::Config::TILESET_PATH;
nbt::TAG_List::ptr_t pkmn::Config::MAPS;
point pkmn::Config::DEF_POS;
DWORD pkmn::Config::DEF_MAP;

namespace pkmn
{
	struct { dav::INI ini; std::string path; } config_file;

	namespace
	{
		point getPoint(const std::string&);
	}

	Config::~Config(void)
	{
		if(!config_file.path.empty())
		{
			std::ofstream o(config_file.path);

			if(!o.is_open())
				throw std::string("couldn't open config file \"" + config_file.path + "\" for write.");

			o << config_file.ini;

			o.close();
		}
	}

	void Config::loadConfig(const std::string& fn)
	{
		std::ifstream in(fn);
		
		LOG("Attempting to read configuration file %s [...]", fn.data());

		if(!in.is_open())
		{
			LOG("Failed. Config file will be created with default values.");
		}
		else
		{
			LOG("Succeeded.");
			in >> config_file.ini;
			in.close();
		}

		config_file.path = fn;

		pInst().load();
	}

	void Config::load(void)
	{
		using dav::aux::lexical_cast;

		dav::INI &ini(config_file.ini);

		ini.addTopic(MXT_TOPIC_GENERAL, true);
		DEF_POS = getPoint(ini.getString(MXT_GENERAL_DEFPOS, MXT_GENERAL_DEFPOS_DEFAULT));
		DEF_MAP = ini.getInt(MXT_GENERAL_DEFMAP, MXT_GENERAL_DEFMAP_DEFAULT);

		ini.addTopic(MXT_TOPIC_RENDER, true);
		TILE_SIZE = ini.getInt(MXT_RENDER_TILESIZE, MXT_RENDER_TILESIZE_DEFAULT);
		SCREEN_WIDTH = ini.getInt(MXT_RENDER_SCREENWIDTH, MXT_RENDER_SCREENWIDTH_DEFAULT);
		SCREEN_HEIGHT = ini.getInt(MXT_RENDER_SCREENHEIGHT, MXT_RENDER_SCREENHEIGHT_DEFAULT);
		SCREEN_SCALE = ini.getInt(MXT_RENDER_SCREENSCALE, MXT_RENDER_SCREENSCALE_DEFAULT);
		TILESET_PATH = ini.getString(MXT_RENDER_TILESETPATH, MXT_RENDER_TILESETPATH_DEFAULT);

		ini.addTopic(MXT_TOPIC_ABSTRACT, true);
		std::string mpath = ini.getString(MXT_ABSTRACT_MAPPATH, MXT_ABSTRACT_MAPPATH_DEFAULT);
		nbt::TAG_Compound::ptr_t tag = nbt::readFile(mpath);
		if(!tag->hasTag(MXT_NBT_MAPS)) throw std::string("no map data found!");
		MAPS = tag->getTagList(MXT_NBT_MAPS);
		if(MAPS->tagID() != nbt::TAG_Compound::ID) throw std::string("map data invalid!");
	}

	namespace
	{
		point getPoint(const std::string& s)
		{
			using dav::aux::lexical_cast;

			auto i = s.find_first_of(",.;: \t/|\\");
			
			if(i == std::string::npos) throw std::string("invalid point '" + s + "'!");
			
			point p;
			
			p.x = lexical_cast<int>(s.substr(0, i));
			p.y = lexical_cast<int>(s.substr(i + 1));
			
			return p;
		}
	}
}

