#ifndef PKMN_RENDER_TSHELPER_H
#define PKMN_RENDER_TSHELPER_H

#include <set>
#include "TileSet.h"

namespace pkmn
{
	namespace render
	{
		class TSHelper
		{
			public:
				static TileSet generate(const std::set<DWORD>&);
			private:
				TSHelper( );
				~TSHelper( );
			private:
				struct Impl;
				Impl *impl_;
			private:
				static TSHelper& instance( ) { static TSHelper t; return t; }
			private:
				TSHelper(const TSHelper&) = delete;
				TSHelper& operator=(const TSHelper&) = delete;
		};
	}
}

#endif

