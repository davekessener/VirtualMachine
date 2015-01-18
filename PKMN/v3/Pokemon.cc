#include "Pokemon.h"
#include "Species.h"
#include "Move.h"
#include "Natures.hpp"
#include "GrowthRates.hpp"

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
				? (ivs[i] + 2 * s.stats[i] + evs[i] / 4 + 100) * level / 100 + 10
				: ((ivs[i] + 2 * s.stats[i] + evs[i] / 4) * level / 100 + 5) * Natures::Multiplier(nature, i);
		}
		health = stats[0];

		int c = 0;
		for(const auto& m : s.moves)
		{
			if(m.lvl > level) break;
			moves[c % 4].id = m.id;
			moves[c % 4].pp = Moves::Get(m.id).pp;
			moves[c % 4].pp_ups = 0;
			++c;
		}

		if(!c) throw std::string("ERR: Pokemon '" + s.name + "' has no attacks!");
	}

	void Pokemon::writeToNBT(nbt::TAG_Compound_ptr_t tag) const
	{
		tag->setString("UUID", id.toString());
		tag->setString("Species", species);
		tag->setString("Nickname", nickname);
		tag->setString("Sprite", sprite);
		tag->setInt("Gender", gender);
		tag->setLong("Personality", personality);
		tag->setInt("Level", level);
		tag->setInt("Experience", exp);
		tag->setInt("Happiness", happiness);
		tag->setString("Ability", ability);
		tag->setIntArray("Stats", stats, 6);
		tag->setIntArray("EVs", evs, 6);
		tag->setIntArray("IVs", ivs, 6);
		tag->setString("Nature", nature);
		tag->setInt("Health", health);
		tag->setString("Status", status);
		
		nbt::TAG_Compound_ptr_t ot_ = nbt::Make<nbt::TAG_Compound>();
		ot_->setString("Name", ot.name);
		ot_->setString("UUID", ot.id.toString());
		tag->setCompoundTag("OT", ot_);

		nbt::TAG_List_ptr_t moves_ = nbt::Make<nbt::TAG_List>();
		for(int i = 0 ; i < 4 ; ++i)
		{
			nbt::TAG_Compound_ptr_t move = nbt::Make<nbt::TAG_Compound>();
			move->setString("ID", moves[i].id);
			move->setInt("PP", moves[i].pp);
			move->setInt("PP-Ups", moves[i].pp_ups);
			moves_->addTag(move);
		}
		tag->setTagList("Moves", moves_);

		nbt::TAG_List_ptr_t forgotten = nbt::Make<nbt::TAG_List>();
		for(const auto& m : forgotten_moves)
		{
			forgotten->addTag(nbt::Make<nbt::TAG_String>("", m));
		}
		tag->setTag("ForgottenMoves", forgotten);
	}

	void Pokemon::readFromNBT(nbt::TAG_Compound_ptr_t tag)
	{
		id = dav::UUID(tag->getString("UUID"));
		species = tag->getString("Species");
		nickname = tag->getString("Nickname");
		sprite = tag->getString("Sprite");
		gender = tag->getInt("Gender");
		personality = tag->getLong("Personality");
		level = tag->getInt("Level");
		exp = tag->getInt("Experience");
		happiness = tag->getInt("Happiness");
		ability = tag->getString("Ability");

		auto stats_ = tag->getIntArray("Stats");
		if(stats_.size() != 6) throw std::string("Invalid stat length in pokemon!");
		std::copy(stats_.cbegin(), stats_.cend(), stats);
		
		
		auto evs_ = tag->getIntArray("EVs");
		if(evs_.size() != 6) throw std::string("Invalid ev length in pokemon!");
		std::copy(evs_.cbegin(), evs_.cend(), evs);
		
		
		auto ivs_ = tag->getIntArray("IVs");
		if(ivs_.size() != 6) throw std::string("Invalid iv length in pokemon!");
		std::copy(ivs_.cbegin(), ivs_.cend(), ivs);

		nature = tag->getString("Nature");
		health = tag->getInt("Health");
		status = tag->getString("Status");

		nbt::TAG_Compound_ptr_t ot_ = tag->getCompoundTag("OT");
		ot.name = ot_->getString("Name");
		ot.id = dav::UUID(ot_->getString("UUID"));

		nbt::TAG_List_ptr_t moves_ = tag->getTagList("Moves"); int c = 0;
		for(auto i(moves_->begin<nbt::TAG_Compound>()), e(moves_->end<nbt::TAG_Compound>()) ; i != e ; ++i)
		{
			moves[c].id = (*i)->getString("ID");
			moves[c].pp = (*i)->getInt("PP");
			moves[c].pp_ups = (*i)->getInt("PP-Ups");
			if(++c > 4) throw std::string("ERR: Too many moves!");
		}

		nbt::TAG_List_ptr_t forgotten = tag->getTagList("ForgottenMoves");
		for(auto i(forgotten->begin<nbt::TAG_String>()), e(forgotten->end<nbt::TAG_String>()) ; i != e ; ++i)
		{
			forgotten_moves.push_back((*i)->get());
		}
	}

	std::string Pokemon::Name(void) const
	{
		return nickname.empty() ? SpeciesManager::Get(species).name : nickname;
	}
}

