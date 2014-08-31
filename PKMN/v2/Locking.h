#ifndef PKMN_LOCKING_H
#define PKMN_LOCKING_H

namespace pkmn
{
	class Locking
	{
		public:
			virtual point position( ) const = 0;
			virtual vec size( ) const = 0;
	};
}

#endif

