#ifndef PKMN_PLAYER_H
#define PKMN_PLAYER_H

#include "common.h"
#include <string>
#include <nbt/NBT.h>
#include "Direction.h"
#include "Locking.h"
#include "render/Sprite.h"

namespace pkmn
{
	class Player : public Locking
	{
		public:
			Player(nbt::TAG_Compound::ptr_t p) : sprite_(0) { load(p); }
			void load(nbt::TAG_Compound::ptr_t);
			nbt::TAG_Compound::ptr_t save( ) const;
			void render(int, int) const;
			void update(int);
			void move(Direction);
			bool isMoving( ) const { return pos_.cur.x != pos_.next.x || pos_.cur.y != pos_.next.y; }
			point position( ) const;
			vec size( ) const { return vec(16, 24); }
			point coords( ) const { return pos_.cur; }
			const std::string& name( ) const { return name_; }
			QWORD ID( ) const { return uid_; }
			DWORD map( ) const { return mapid_; }
			static nbt::TAG_Compound::ptr_t generate(const std::string&);
		private:
			std::string name_;
			QWORD uid_;
			struct { point cur, next; uint step; struct { Direction dir; int v; } prog; } pos_;
			DWORD mapid_;
			render::Sprite sprite_;
	};
}

#endif

