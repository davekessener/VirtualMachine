#ifndef PKMN_EDITOR_TILESET_H
#define PKMN_EDITOR_TILESET_H

#include "../common.h"

namespace editor
{
	class Tileset
	{
		public:
			static void bind( ) { instance().doBind(); }
			static void set(DWORD id) { instance().doSet(id); }
			static DWORD get( ) { return instance().doGet(); }
			static bool GRID;
		private:
			Tileset( );
			void doBind( ) const;
			void doSet(DWORD);
			DWORD doGet( ) const;
			static Tileset& instance( ) { static Tileset ts; return ts; }
		private:
			DWORD id_, cur_;
		private:
			Tileset(const Tileset&) = delete;
			Tileset& operator=(const Tileset&) = delete;
	};
}

#endif

