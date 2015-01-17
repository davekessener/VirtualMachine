#ifndef PKMN_BATTLE_BATTLE_H
#define PKMN_BATTLE_BATTLE_H

#include "common.h"
#include "../UUID.hpp"
#include "../Pokemon.hpp"

namespace pkmn
{
	namespace battle
	{
		class Battle
		{
			public:
				void registerTrainers(dav::UUID, dav::UUID);
				void switchPokemon(dav::UUID, Pokemon_ptr);
			private:
		};
	}
}

#endif

