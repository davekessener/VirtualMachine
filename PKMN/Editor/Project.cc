#include "Project.h"

namespace editor
{
	Project::Project(const std::string& name) : _fn(name), _nextID(0)
	{
		load();
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

	void Project::save(void)
	{
		std::ofstream o(_fn, std::ios::out | std::ios::binary);

		if(!o.is_open())
		{
			throw SDLException("File '%s' couldn't be opened!", _fn.c_str());
		}

		nbt::TAG_Compound::ptr_t nbt = nbt::Make<nbt::TAG_Compound>();
		nbt::TAG_List::ptr_t list = nbt::Make<nbt::TAG_List>(Settings::NBT_MAPS);

		for(std::shared_ptr<MapData>& map : _maps)
		{
			list->addTag(map->save());
		}

		nbt->setTagList(list);
		nbt->write(o);

		o.close();
	}

	void Project::load(void)
	{
		std::ifstream in(_fn, std::ios::in | std::ios::binary);

		if(!in.is_open())
		{
			LOG("New project created.");
			return;
		}

		nbt::TAG_Compound::ptr_t nbt = nbt::Read(in);
		nbt::TAG_List::ptr_t list = nbt->getTagList(Settings::NBT_MAPS);

		in.close();

		for(auto i = list->begin<nbt::TAG_Compound>() ; i != list->end<nbt::TAG_Compound>() ; ++i)
		{
			_maps.push_back(std::shared_ptr<MapData>(MapData::load(*i)));
		}
	}
}

