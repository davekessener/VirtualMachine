#ifndef PKMN_BATTLE_POKEMON_H
#define PKMN_BATTLE_POKEMON_H

#include "common.h"
#include "../Pokemon.h"
#include "../Species.h"
#include "../Move.h"
#include "../Natures.hpp"
#include "../Types.hpp"

namespace pkmn
{
	namespace battle
	{
		struct Pokemon
		{
			std::string name;
			Species_ptr species;
			std::string types[2];
			int gender;
			int stats[6];
			int stat_modifier[8];
			int health;
//			Status_ptr status_primary;
//			std::vector<Status_ptr> status_secondary;
			struct
			{
				Move_ptr move;
				int pp, pp_max;
			} moves[4];
//			Item_ptr item;

			static Pokemon Create(::pkmn::Pokemon_ptr);
		};
	}
}

#endif

