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
			static std::string SPRITE_PATH;
			static uint SPRITE_PHASES;
			static nbt::TAG_List::ptr_t MAPS;
			static point DEF_POS;
			static DWORD DEF_MAP;
			static DWORD KEY_UP;
			static DWORD KEY_DOWN;
			static DWORD KEY_LEFT;
			static DWORD KEY_RIGHT;
			static DWORD KEY_A;
			static DWORD KEY_B;
			static DWORD KEY_L;
			static DWORD KEY_R;
			static DWORD KEY_START;
			static DWORD KEY_SELECT;
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

