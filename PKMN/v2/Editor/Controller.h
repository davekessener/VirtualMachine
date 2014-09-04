#ifndef PKMN_EDITOR_CONTROLLER_H
#define PKMN_EDITOR_CONTROLLER_H

#include "../common.h"
#include "File.h"

namespace editor
{
	class Controller
	{
		typedef File::map_t map_t;

		public:
			static void load(QWORD);
			static bool isLoaded( );
			static const map_t& getMap( );
			static void set(uint, uint, uint, DWORD);
			static void setBuffer(uint, uint, uint, DWORD);
			static void commitBuffer( );
			static DWORD get(uint, uint, uint);
			static void undo( );
			static void redo( );
			static bool hasChanged( );
			static void save( );
			static bool GRID;
			static bool SPLIT;
		private:
			Controller( ) = default;
			~Controller( );
			void doLoad(QWORD);
			static Controller& instance( ) { static Controller c; return c; }
		private:
			struct Impl;
			Impl *impl_;
		private:
			Controller(const Controller&) = delete;
			Controller& operator=(const Controller&) = delete;
	};
}

#endif

