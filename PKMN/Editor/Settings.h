#ifndef EDITOR_SETTINGS_H
#define EDITOR_SETTINGS_H

#include <string>
#include <map>
#include <Surface/Image.h>

namespace editor
{
	class Settings
	{
		public:
			static bool getBool(const std::string&);
			static void setBool(const std::string&, bool);
			static Image *getTileset( );
			static const std::string TS_GRID;
			static const std::string MAP_GRID;
			static const std::string MAP_BLUR;
			static const std::string TS_BUTTON_GRID;
			static const std::string MAP_BUTTON_BLUR;
			static const std::string NBT_MAP_NAME;
			static const std::string NBT_MAP_ID;
			static const std::string NBT_MAP_WIDTH;
			static const std::string NBT_MAP_HEIGHT;
			static const std::string NBT_MAP_DATA;
			static const std::string NBT_MAP_DATA_BOTTOM;
			static const std::string NBT_MAP_DATA_INTER;
			static const std::string NBT_MAP_DATA_ANIMATION;
			static const std::string NBT_MAP_DATA_TOP;
			static const std::string NBT_MAPS;
			static const std::string TS_FN;
		private:
			Settings( );
			~Settings( );
			static Settings& instance( ) { static Settings s; return s; }
			std::map<std::string, bool> boolMap;
			Image *_ts;
	};
}

#endif

