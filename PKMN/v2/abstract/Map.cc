#include "Map.h"
#include <aux>

#define MXT_ID "ID"
#define MXT_NAME "Name"
#define MXT_WIDTH "Width"
#define MXT_HEIGHT "Height"
#define MXT_DATA "Data"
#define MXT_BOTTOM "Bottom"
#define MXT_ANIMATION "Animation"
#define MXT_INTERMEDIATE "Intermediate"
#define MXT_TOP "Top"

namespace pkmn { namespace abstract {

Map::Map(nbt::TAG_Compound::ptr_t tag)
{
	using dav::aux::lexical_cast;

	bool noid = false, noname = false, now = false, noh = false;

	if(!tag->hasTag(MXT_ID)) noid = true;
	else id_ = tag->getLong(MXT_ID);

	if(!tag->hasTag(MXT_NAME)) noname = true;
	else name_ = tag->getString(MXT_NAME);

	if(!tag->hasTag(MXT_WIDTH)) now = true;
	else width_ = tag->getInt(MXT_WIDTH);

	if(!tag->hasTag(MXT_HEIGHT)) noh = true;
	else height_ = tag->getInt(MXT_HEIGHT);

	if(noid)
	{
		std::string s = "map ";
		s += (noname ? "<no-name>" : ("\"" + name_ + "\""));
		s += " has no id!";
		throw s;
	}

	if(noname)
	{
		throw std::string("map #" + lexical_cast<std::string>(id_) + " has no name!");
	}

	if(now||noh)
	{
		throw std::string("map #" + lexical_cast<std::string>(id_) + " \"" + name_ + "\" has incorrect size!");
	}

	if(!tag->hasTag(MXT_DATA))
		throw std::string("map #" + lexical_cast<std::string>(id_) + " \"" + name_ + "\" has incorrect size!");
	
	tag = tag->getCompoundTag(MXT_DATA);

	std::vector<std::string> l{MXT_BOTTOM, MXT_ANIMATION, MXT_INTERMEDIATE, MXT_TOP};
	vec_t *p = layers_;
	bool *e = empty_;

	for(const std::string& i : l)
	{
		if(tag->hasTag(i))
		{
			const auto &a = tag->getIntArray(i);
			vec_t(a.cbegin(), a.cend()).swap(*p);
			if(p->size() != width_ * height_)
			{
				throw std::string("map #" + lexical_cast<std::string>(id_) + " \"" + name_ 
							+ "\"' " + i + " has incorrect size!");
			}

			*e = true;
			for(const DWORD& v : *p) if(v) { *e = false; break; }
		}
		else
		{
			p->resize(width_ * height_);
		}

		++p;
		++e;
	}
}

}}

