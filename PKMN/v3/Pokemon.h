#ifndef PKMN_GAME_POKEMON_H
#define PKMN_GAME_POKEMON_H

#include <vector>
#include "common.h"
#include "UUID.hpp"
#include <nbt/NBT.h>

namespace pkmn
{
	class Pokemon
	{
		public:
		typedef std::shared_ptr<Pokemon> Pokemon_ptr;

		public:
			static Pokemon_ptr Generate(const std::string&, uint);
		public:
			dav::UUID id;
			std::string species;
			std::string nickname;
			std::string sprite;
			int gender;
			QWORD personality;
			uint level;
			uint exp;
			uint happiness;
			std::string ability;
			uint stats[6];
			uint evs[6];
			uint ivs[6];
			std::string nature;
			uint health;
			std::string status;
			struct
			{
				std::string name;
				dav::UUID id;
			} ot;
			struct
			{
				std::string id;
				uint pp;
				uint pp_ups;
			} moves[4];
			std::vector<std::string> forgotten_moves;
		public:
			void writeToNBT(nbt::TAG_Compound_ptr_t) const;
			void readFromNBT(nbt::TAG_Compound_ptr_t);
		public:
			Pokemon( );
			Pokemon(const std::string&, uint);
	};

	typedef Pokemon::Pokemon_ptr Pokemon_ptr;
}

#endif

