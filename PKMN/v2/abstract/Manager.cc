#include "Manager.h"
#include "../Config.h"
#include <dav/Logger.h>

namespace pkmn { namespace abstract {

Manager::Manager(void)
{
	nbt::TAG_List::ptr_t list = Config::MAPS;

	for(auto i = list->begin<nbt::TAG_Compound>(), e = list->end<nbt::TAG_Compound>() ; i != e ; ++i)
	{
		Map_ptr p(new Map(*i));

		if(map_.find(p->ID()) != map_.end())
		{
			LOG("ERR: Duplicate ids %u (%s)", p->ID(), p->name().data());
			throw std::string("duplicate id!");
		}
		
		map_[p->ID()] = p;
	}

	if(map_.find(0) == map_.end())
	{
		LOG("ERR: No zero-id map!");
		throw std::string("map with id 0 is missing");
	}
}

}}

