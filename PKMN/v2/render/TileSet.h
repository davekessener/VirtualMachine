#ifndef PKMN_RENDER_TILESET_H
#define PKMN_RENDER_TILESET_H

#include <dav/inc.h>
#include <map>

namespace pkmn
{
	namespace render
	{
		class TileSet
		{
			public:
			typedef std::map<DWORD, DWORD> tbl_t;

			public:
				TileSet( ) = default;
				TileSet(DWORD id, int ds, tbl_t&& tbl) : id_(id), ds_(ds), tbl_(tbl) { }
				void bind( ) const;
				void render(int, int, DWORD) const;
			private:
				DWORD id_;
				int ds_;
				tbl_t tbl_;
		};
	}
}

#endif

