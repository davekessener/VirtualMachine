#include "Player.h"
#include <random>
#include "Config.h"

#define MXT_NAME "Name"
#define MXT_UID "UID"
#define MXT_POSX "PosX"
#define MXT_POSY "PosY"
#define MXT_MAPID "Map"

namespace pkmn
{
	namespace
	{
		QWORD genUID( );
	}

	Player::Player(const std::string& name)
		: name_(name), uid_(genUID()), pos_(Config::DEF_POS), npos_(pos_), mapid_(Config::DEF_MAP)
	{
	}

	void Player::load(nbt::TAG_Compound::ptr_t tag)
	{
		if(!tag->hasTag(MXT_NAME)) throw std::string("player has no name!");
		if(!tag->hasTag(MXT_UID)) throw std::string("player has no id!");
		if(!tag->hasTag(MXT_POSX) || !tag->hasTag(MXT_POSY)) throw std::string("player has no pos!");
		if(!tag->hasTag(MXT_MAPID)) throw std::string("player has no map!");

		name_ = tag->getString(MXT_NAME);
		uid_ = tag->getLong(MXT_UID);
		pos_.x = npos_.x = tag->getInt(MXT_POSX);
		pos_.y = npos_.y = tag->getInt(MXT_POSY);
		mapid_ = tag->getInt(MXT_MAPID);
	}

	nbt::TAG_Compound::ptr_t Player::save(void) const
	{
		nbt::TAG_Compound::ptr_t tag = nbt::Make<nbt::TAG_Compound>();
		tag->setString(MXT_NAME, name_);
		tag->setLong(MXT_UID, uid_);
		tag->setInt(MXT_POSX, pos_.x);
		tag->setInt(MXT_POSY, pos_.y);
		tag->setInt(MXT_MAPID, mapid_);
		return tag;
	}

	namespace
	{
		QWORD genUID(void)
		{
			std::random_device seed;
			std::mt19937_64 gen(seed());
			std::uniform_int_distribution<QWORD> dist(0, static_cast<QWORD>(-1));

			return dist(gen);
		}
	}
}

