#include "Pokemon.h"

namespace pkmn { namespace battle {

Pokemon Pokemon::Create(::pkmn::Pokemon_ptr poke)
{
	Pokemon p;
	Species_ptr sp = &SpeciesManager::Get(poke->species);

	p.name = poke->nickname.empty() ? sp->name : poke->nickname;
	p.species = sp;
	p.types[0] = sp->types[0];
	p.types[1] = sp->types[1];
	p.gender = poke->gender;
	p.level = poke->level;
	
	for(int i = 0 ; i < 6 ; ++i)
	{
		p.stats[i] = poke->stats[i];
	}

	for(int i = 0 ; i < 8 ; ++i)
	{
		p.stat_modifier[i] = 0;
	}

	p.health = poke->health;

	for(int i = 0 ; i < 4 ; ++i)
	{
		if(poke->moves[i].id.empty())
		{
			p.moves[i].move = nullptr;
			p.moves[i].pp = p.moves[i].pp_max = 0;
		}
		else
		{
			p.moves[i].move = &Moves::Get(poke->moves[i].id);
			p.moves[i].pp = poke->moves[i].pp;
			p.moves[i].pp_max = p.moves[i].move->pp * (5 + poke->moves[i].pp_ups) / 5;
		}
	}

	return p;
}

int Pokemon::applyStat(uint i, int s)
{
	if(i < 2 || i > 7) throw std::string("ERR: invalid stat to modify!");

	if(s == 0) throw std::string("ERR: can't not modify stat (WUT)!");

	if(s < 0)
	{
		if(stat_modifier[i] + s < -6) s = -6 - stat_modifier[i];
	}
	else
	{
		if(stat_modifier[i] + s > 6) s = 6 - stat_modifier[i];
	}

	stat_modifier[i] += s;

	return s;
}

}}

