#include "Player.h"
#include <random>
#include <dav/Logger.h>
#include <aux>
#include "Config.h"

#define MXT_NAME "Name"
#define MXT_UID "UID"
#define MXT_POSX "PosX"
#define MXT_POSY "PosY"
#define MXT_MAPID "Map"

#define MXT_SPEED 300

namespace pkmn
{
	nbt::TAG_Compound::ptr_t Player::generate(const std::string& name)
	{
		nbt::TAG_Compound::ptr_t tag = nbt::Make<nbt::TAG_Compound>();
		tag->setString(MXT_NAME, name);
		tag->setLong(MXT_UID, dav::aux::generateUID<QWORD>());
		tag->setInt(MXT_POSX, Config::DEF_POS.x);
		tag->setInt(MXT_POSY, Config::DEF_POS.y);
		tag->setInt(MXT_MAPID, Config::DEF_MAP);
		return tag;
	}

	void Player::load(nbt::TAG_Compound::ptr_t tag)
	{
		if(!tag->hasTag(MXT_NAME)) throw std::string("player has no name!");
		if(!tag->hasTag(MXT_UID)) throw std::string("player has no id!");
		if(!tag->hasTag(MXT_POSX) || !tag->hasTag(MXT_POSY)) throw std::string("player has no pos!");
		if(!tag->hasTag(MXT_MAPID)) throw std::string("player has no map!");

		name_ = tag->getString(MXT_NAME);
		uid_ = tag->getLong(MXT_UID);
		pos_.cur.x = pos_.next.x = tag->getInt(MXT_POSX);
		pos_.cur.y = pos_.next.y = tag->getInt(MXT_POSY);
		pos_.step = 0;
		pos_.prog.v = 0;
		pos_.prog.dir = Direction::DOWN;
		mapid_ = tag->getInt(MXT_MAPID);
	}

	nbt::TAG_Compound::ptr_t Player::save(void) const
	{
		nbt::TAG_Compound::ptr_t tag = nbt::Make<nbt::TAG_Compound>();
		tag->setString(MXT_NAME, name_);
		tag->setLong(MXT_UID, uid_);
		tag->setInt(MXT_POSX, pos_.cur.x);
		tag->setInt(MXT_POSY, pos_.cur.y);
		tag->setInt(MXT_MAPID, mapid_);
		return tag;
	}

	void Player::render(int dx, int dy) const
	{
		point p(position());
		float f(pos_.prog.v / (float)MXT_SPEED);

		sprite_.render(pos_.prog.dir, (pos_.step % 2) ? f : -f, dx + p.x, dy + p.y - 8);
	}

	void Player::update(int d)
	{
		if(isMoving())
		{
			if((pos_.prog.v += d) >= MXT_SPEED)
			{
				pos_.cur = pos_.next;
				pos_.prog.v = 0;
				++pos_.step;
			}
		}
	}

	void Player::move(Direction d)
	{
		if(isMoving()) return;

		switch(d)
		{
			case Direction::UP:
				pos_.next.y = pos_.cur.y - 1;
				break;
			case Direction::DOWN:
				pos_.next.y = pos_.cur.y + 1;
				break;
			case Direction::LEFT:
				pos_.next.x = pos_.cur.x - 1;
				break;
			case Direction::RIGHT:
				pos_.next.x = pos_.cur.x + 1;
				break;
		}

		pos_.prog.dir = d;
	}

	point Player::position(void) const
	{
		int x = pos_.cur.x * 16, y = pos_.cur.y * 16;
		if(pos_.cur.x != pos_.next.x) x += ((pos_.next.x - pos_.cur.x) * 16) * pos_.prog.v / MXT_SPEED;
		if(pos_.cur.y != pos_.next.y) y += ((pos_.next.y - pos_.cur.y) * 16) * pos_.prog.v / MXT_SPEED;
		return point(x, y);
		
//		auto lint = [](int i, int j, float p) { return i + (j - i) * p; };
//		float f = pos_.prog / (float)MXT_SPEED;
//
//		if(f <= 0.0) return fpoint(pos_.cur.x, pos_.cur.y);
//		else if(f >= 1.0) return fpoint(pos_.next.x, pos_.next.y);
//		
//		return fpoint(lint(pos_.cur.x, pos_.next.x, f),
//					  lint(pos_.cur.y, pos_.next.y, f));
	}
}

