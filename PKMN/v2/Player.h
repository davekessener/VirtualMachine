#ifndef PKMN_PLAYER_H
#define PKMN_PLAYER_H

#include "common.h"
#include <string>
#include <nbt/NBT.h>

namespace pkmn
{
	class Player
	{
		public:
			Player(const std::string&);
			Player(nbt::TAG_Compound::ptr_t p) { load(p); }
			void load(nbt::TAG_Compound::ptr_t);
			nbt::TAG_Compound::ptr_t save( ) const;
		private:
			std::string name_;
			QWORD uid_;
			point pos_, npos_;
			DWORD mapid_;
	};
}

#endif

