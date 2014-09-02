#ifndef PKMN_EDITOR_TILESET_H
#define PKMN_EDITOR_TILESET_H

#include "../common.h"

namespace editor
{
	class Tileset
	{
		public:
			static void bind( ) { instance().doBind(); }
		private:
			Tileset( );
			~Tileset( );
			void doBind( ) const;
			static Tileset& instance( ) { static Tileset ts; return ts; }
		private:
			DWORD id_;
		private:
			Tileset(const Tileset&) = delete;
			Tileset& operator=(const Tileset&) = delete;
	};
}

#endif

