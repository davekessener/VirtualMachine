#ifndef PKMN_ABSTRACT_MANAGER_H
#define PKMN_ABSTRACT_MANAGER_H

#include "Map.h"

namespace pkmn
{
	namespace abstract
	{
		class Manager
		{
			typedef std::shared_ptr<Map> Map_ptr;

			public:
				static const Map& getMap(DWORD id) { return *instance().map_.at(id); }
			private:
				static Manager& instance( ) { static Manager m; return m; }
			private:
				Manager( );
				~Manager( ) = default;
			private:
				std::map<DWORD, Map_ptr> map_;
		};
	}
}

#endif

