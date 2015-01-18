#include "Species.h"

namespace pkmn
{
	void SpeciesManager::load(nbt::TAG_List_ptr_t list)
	{
		for(auto i(list->begin<nbt::TAG_Compound>()), e(list->end<nbt::TAG_Compound>()) ; i != e ; ++i)
		{
			Species_sptr s = Read(*i);
			species_[s->id] = s;
		}
	}

	Species_sptr SpeciesManager::get(const std::string& id) const
	{
		auto i(species_.find(id));

		if(i == species_.cend()) throw std::string("ERR: Unknown species'" + id + "'!");

		return i->second;
	}

	Species_sptr SpeciesManager::Read(nbt::TAG_Compound_ptr_t tag)
	{
		Species_sptr s = std::make_shared<Species>();
		
		s->id = "#ERR#";

		try
		{
			s->id = tag->getString("ID");
			s->name = tag->getString("Name");
			
			nbt::TAG_List_ptr_t types = tag->getTagList("Types"); int c = 0;
			for(auto i(types->begin<nbt::TAG_String>()), e(types->end<nbt::TAG_String>()) ; i != e ; ++i)
			{
				s->types[c] = i->get();
				if(++c > 2) throw std::string("ERR: Too many types in species '" + s->id + "'!");
			}
			
			nbt::TAG_List_ptr_t abilities = tag->getTagList("Abilities"); c = 0;
			for(auto i(abilities->begin<nbt::TAG_String>()), e(abilities->end<nbt::TAG_String>()) ; i != e ; ++i)
			{
				s->abilities[c] = i->get();
				if(++c > 2) throw std::string("ERR: Too many abilities in species '" + s->id + "'!");
			}

			s->hidden_ability = tag->getString("HiddenAbility");
			s->gender_ratio = tag->getFloat("GenderRatio");
			s->is_genderless = tag->getByte("IsGenderless");
			s->is_baby = tag->getByte("IsBaby");
			s->catch_rate = tag->getFloat("CatchRate");
			s->egg_cycles = tag->getInt("EggCycles");
			s->base_exp = tag->getInt("BaseExperience");
			s->base_happiness = tag->getInt("BaseHappiness");
			s->growth_rate = tag->getInt("GrowthRate");
			
			nbt::TAG_List_ptr_t egg_groups = tag->getTagList("EggGroups"); c = 0;
			for(auto i(egg_groups->begin<nbt::TAG_String>()), e(egg_groups->end<nbt::TAG_String>()) ; i != e ; ++i)
			{
				s->egg_groups[c] = i->get();
				if(++c > 2) throw std::string("ERR: Too many egg groups in species '" + s->id + "'!");
			}

			std::vector<BYTE> stats = tag->getByteArray("Stats");
			if(stats.size() != 6) throw std::string("ERR: Too many stats in species '" + s->id + "'!");
			std::copy(stats.cbegin(), stats.cend(), s->stats);

			std::vector<BYTE> evs = tag->getByteArray("EVs");
			if(evs.size() != 6) throw std::string("ERR: Too many evs in species '" + s->id + "'!");
			std::copy(evs.cbegin(), evs.cend(), s->evs);

			s->tms = tag->getByteArray("TMs");
			s->hms = tag->getByteArray("HMs");

			nbt::TAG_List_ptr_t moves = tag->getTagList("Moves");
			for(auto i(moves->begin<nbt::TAG_Compound>()), e(moves->end<nbt::TAG_Compound>()) ; i != e ; ++i)
			{
				Species::Moves m;

				m.id = (*i)->getString("ID");
				m.lvl = (*i)->getInt("Level");

				s->moves.push_back(m);
			}

			nbt::TAG_Compound_ptr_t dex = tag->getCompoundTag("Dex");
			s->dex.number = dex->getInt("Number");
			s->dex.height = dex->getInt("Height");
			s->dex.weight = dex->getInt("Weight");
			s->dex.color = dex->getString("Color");
		}
		catch(const nbt::NBTException& e)
		{
			throw std::string("ERR: While reading species '" + s->id + "': '" + std::string(e.what()) + "'!");
		}

		return s;
	}
}

