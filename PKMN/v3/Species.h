#ifndef PKMN_GAME_SPECIES_H
#define PKMN_GAME_SPECIES_H

#include <string>
#include <vector>
#include "common.h"
#include <nbt/NBT.h>

namespace pkmn
{
	struct Species
	{
		struct Moves
		{
			std::string id;
			uint lvl;
		};

		std::string id;
		std::string name;
		std::string types[2];
		std::string abilities[2];
		std::string hidden_ability;
		float gender_ratio;
		bool is_genderless;
		bool is_baby;
		float catch_rate;
		std::string egg_groups[2];
		uint egg_cycles;
		uint base_exp;
		uint base_happiness;
		uint growth_rate;
		uint stats[6];
		uint evs[6];
		std::vector<BYTE> tms, hms;
		std::vector<Moves> moves;
		struct
		{
			uint number;
			uint height;
			uint weight;
			std::string color;
		} dex;
	};

	typedef std::shared_ptr<Species> Species_sptr;
	typedef const Species *Species_ptr;

	class SpeciesManager
	{
		public:
			static void Load(nbt::TAG_List_ptr_t list) { Instance().load(list); }
			static const Species& Get(const std::string& id) { return *Instance().get(id); }
		private:
			void load(nbt::TAG_List_ptr_t);
			Species_sptr get(const std::string&) const;
			static Species_sptr Read(nbt::TAG_Compound_ptr_t);
		private:
			std::map<std::string, Species_sptr> species_;
		private:
			static SpeciesManager& Instance( ) { static SpeciesManager sm; return sm; }
		private:
			SpeciesManager( ) = default;
			~SpeciesManager( ) = default;
			SpeciesManager(const SpeciesManager&) = delete;
			SpeciesManager& operator=(const SpeciesManager&) = delete;
	};
}

#endif

