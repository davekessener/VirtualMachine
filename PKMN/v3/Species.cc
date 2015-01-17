#include "Species.h"

namespace pkmn
{
	void SpeciesManager::load(nbt::TAG_List_ptr_t list)
	{
		for(auto i(list->begin<nbt::TAG_Compound>()), e(list->end<nbt::TAG_Compound>()) ; i != e ; ++i)
		{
			Species_ptr s = Read(*i);
			species_[s->id] = s;
		}
	}

	Species_ptr SpeciesManager::get(const std::string& id) const
	{
		auto i(species_.find(id));

		if(i == species_.cend()) throw std::string("ERR: Unknown species'" + id + "'!");

		return i->second;
	}

	Species_ptr SpeciesManager::Read(nbt::TAG_Compound_ptr_t tag)
	{
		Species_ptr s(std::make_shared<Species>());
		
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
			
			nbt::TAG_List_ptr_t stats = tag->getTagList("Stats"); c = 0;
			for(auto i(stats->begin<nbt::TAG_Int>()), e(stats->end<nbt::TAG_Int>()) ; i != e ; ++i)
			{
				s->stats[c] = i->get();
				if(++c > 6) throw std::string("ERR: Too many evs in species '" + s->id + "'!");
			}

			nbt::TAG_List_ptr_t evs = tag->getTagList("EVs"); c = 0;
			for(auto i(evs->begin<nbt::TAG_Int>()), e(evs->end<nbt::TAG_Int>()) ; i != e ; ++i)
			{
				s->evs[c] = i->get();
				if(++c > 6) throw std::string("ERR: Too many evs in species '" + s->id + "'!");
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

