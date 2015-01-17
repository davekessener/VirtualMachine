#include "Pokemon.h"
#include "Species.h"
#include "Natures.hpp"

namespace pkmn
{
	Pokemon_ptr Pokemon::Generate(const std::string& sid, uint lvl)
	{
		return std::make_shared<Pokemon>(sid, lvl);
	}

	Pokemon::Pokemon(void)
		: id(dav::UUID::Nil())
		, species("")
		, nickname("")
		, sprite("")
		, gender(0)
		, personality(0)
		, level(0)
		, exp(0)
		, happiness(0)
		, ability("")
		, nature("")
		, health(0)
		, status("")
	{
		for(int i = 0 ; i < 6 ; ++i)
		{
			stats[i] = 0;
			evs[i] = 0;
			ivs[i] = 0;
		}

		ot.name = "";
		ot.id = dav::UUID::Nil();

		for(int i = 0 ; i < 4 ; ++i)
		{
			moves[i].id = "";
			moves[i].pp = moves[i].pp_ups = 0;
		}
	}

	Pokemon::Pokemon(const std::string& sp, uint lvl)
		: id()
		, species(sp)
		, nickname("")
		, sprite(species)
		, personality(dav::UUID::rand<QWORD>())
		, level(lvl)
		, nature(Natures::Random())
		, status("")
	{
		const Species &s = SpeciesManager::Get(species);

		gender = s.is_genderless ? 0 : (dav::UUID::Bool(s.gender_ratio) ? 1 : -1);
		exp = GrowthRates::Get(s.growth_rate).nextLevel(level);
		happiness = s.base_happiness;
		ability = s.abilities[0];
		if(!s.abilities[1].empty() && dav::UUID::Bool(0.5)) ability = s.abilities[1];

		for(int i = 0 ; i < 6 ; i++)
		{
			evs[i] = 0;
			ivs[i] = dav::UUID::rand<uint>() % 32;
			stats[i] = i == 0
				? (ivs[i] + 2 * s.base_stats[i] + evs[i] / 4 + 100) * level / 100 + 10
				: ((ivs[i] + 2 * s.base_stats[i] + evs[i] / 4) * level / 100 + 5) * Natures::Multiplier(i);
		}
		health = stats[0];
	}
}

