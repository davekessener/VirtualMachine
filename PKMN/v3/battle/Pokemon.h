#ifndef PKMN_BATTLE_POKEMON_H
#define PKMN_BATTLE_POKEMON_H

#include <cassert>
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
			Species_ptr species = nullptr;
			std::string types[2];
			int gender;
			uint level;
			uint stats[6];
			int stat_modifier[8];
			uint health;
//			Status_ptr status_primary;
//			std::vector<Status_ptr> status_secondary;
			struct
			{
				Move_ptr move;
				int pp, pp_max;
			} moves[4];
//			Item_ptr item;

			uint HP( ) const { return getStat(0); }
			uint Health( ) const { return health; }
			uint Atk( ) const { return getStat(1); }
			uint Def( ) const { return getStat(2); }
			uint SpA( ) const { return getStat(3); }
			uint SpD( ) const { return getStat(4); }
			uint Spe( ) const { return getStat(5); }
			float Accuracy( ) const { return stageMod(stat_modifier[6]); }
			float Evasion( ) const { return stageMod(stat_modifier[7]); }
			uint getStat(uint i) const
				{ assert(i<6); return i == 0 ? stats[0] : (uint)(stats[i] * stageMod(stat_modifier[i])); }
			float stageMod(int i) const { return i < 0 ? 1.0 / stageMod(-i) : 1.0 + 0.5 * i; }

			operator bool( ) const { return static_cast<bool>(species); }
			uint getAttack(const std::string& sid) const
			{
				for(uint i = 0 ; i < 4 ; ++i)
				{
					if(moves[i].move->id == sid) return i; 
				}

				throw std::string("ERR: " + name + " has no attack named '" + sid + "'!");
			}
			static Pokemon Create(::pkmn::Pokemon_ptr);
		};
	}
}

#endif

