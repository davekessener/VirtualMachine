#ifndef PKMN_CONFIG_H
#define PKMN_CONFIG_H

#include "common.h"
#include <string>
#include <nbt/NBT.h>

namespace pkmn
{
	class Config
	{
		typedef unsigned int uint;

		public:
			static void loadConfig(const std::string&);
			static uint TILE_SIZE;
			static uint SCREEN_WIDTH, SCREEN_HEIGHT;
			static uint SCREEN_SCALE;
			static std::string TILESET_PATH;
			static nbt::TAG_List::ptr_t MAPS;
			static point DEF_POS;
			static DWORD DEF_MAP;
		private:
			Config( ) { }
			~Config( );
			void load( );
		private:
			static Config& pInst( ) { static Config c; return c; }
		private:
			Config(const Config&) = delete;
			Config& operator=(const Config&) = delete;
	};
}

#endif

