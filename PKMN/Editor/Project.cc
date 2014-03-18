#include "Project.h"

namespace editor
{
	Project::Project(const std::string& name) : _fn(name), _nextID(0)
	{
	}

	Project::~Project(void)
	{
	}

	Project::map_id Project::createMap(const std::string& name, int w, int h)
	{
		map_id id = _nextID++;

		_maps.push_back(std::shared_ptr<MapData>(new MapData(id, name, w, h)));

		return id;
	}

	Project::map_ptr Project::getMap(map_id id)
	{
		for(auto i = _maps.begin() ; i != _maps.end() ; ++i)
		{
			if((*i)->ID == id) return *i;
		}

		assert(!"ERR: Map not found!");
	}
}

