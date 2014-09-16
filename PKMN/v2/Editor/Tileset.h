#ifndef PKMN_EDITOR_TILESET_H
#define PKMN_EDITOR_TILESET_H

#include "../common.h"
#include "TileBlock.h"

namespace editor
{
	class Tileset
	{
		public:
			static void bind( ) { instance().doBind(); }
			static void set(DWORD id) { instance().doSet(TileBlock(id, id)); }
			static void set(DWORD id1, DWORD id2) { instance().doSet(TileBlock(id1, id2)); }
			static void set(const TileBlock& tb) { instance().doSet(tb); }
			static TileBlock get( ) { return instance().doGet(); }
			static bool GRID;
		private:
			Tileset( );
			void doBind( ) const;
			void doSet(const TileBlock&);
			TileBlock doGet( ) const;
			static Tileset& instance( ) { static Tileset ts; return ts; }
		private:
			DWORD id_;
			TileBlock cur_;
		private:
			Tileset(const Tileset&) = delete;
			Tileset& operator=(const Tileset&) = delete;
	};
}

#endif

