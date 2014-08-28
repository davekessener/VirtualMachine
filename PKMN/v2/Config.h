#ifndef PKMN_CONFIG_H
#define PKMN_CONFIG_H

#include <string>

namespace pkmn
{
	class Config
	{
		typedef unsigned int uint;

		public:
			static const Config& instance( ) { return pInst(); }
			static void loadConfig(const std::string&);
			uint TILE_SIZE;
			uint SCREEN_WIDTH, SCREEN_HEIGHT;
			uint SCREEN_SCALE;
			std::string TILESET_PATH;
			std::string MAP_PATH;
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

