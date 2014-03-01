#define CONTROLLER_MAIN
#include "Controller.h"
#undef CONTROLLER_MAIN

Controller *Controller::_instance = NULL;

void Controller::create(const std::string& path)
{
	assert(!_instance);

	_instance = new Controller(path);
	MapRenderer::init();
}

void Controller::destroy(void)
{
	assert(_instance);

	MapRenderer::quit();
	delete _instance;
	_instance = NULL;
}

Controller& Controller::instance(void)
{
	assert(_instance);
	return *_instance;
}

// # ---------------------------------------------------------------------------

void Controller::render(void)
{
	MapRenderer &render = *MapRenderer::getRenderer(*maps.at(curMap));
	render.renderAt(0, 0);
}

// # ===========================================================================

Controller::Controller(const std::string& _saveFn) : saveFn(_saveFn), curMap(0)
{
	nbt::TAG_Compound::ptr_t nbt = nbt::readFile(saveFn);
	nbt::TAG_List::ptr_t maps = nbt->getTagList("Maps");

	for(auto i = maps->begin<nbt::TAG_Compound>() ; i != maps->end<nbt::TAG_Compound>() ; ++i)
	{
		nbt::TAG_Compound::ptr_t map_ptr = *i;

		Map *map = new Map(map_ptr->getLong("ID"), map_ptr->getString("Name"), 
			map_ptr->getInt("Width"), map_ptr->getInt("Height"));

		nbt::TAG_Compound::ptr_t data = map_ptr->getCompoundTag("Data");

		map->setBottomLayer(data->getIntArray("Bottom"));
		map->setAnimationLayer(data->getIntArray("Animation"));
		map->setTopLayer(data->getIntArray("Top"));

		this->maps.push_back(map);

		LOG("Read map #%d: '%s', %d x %d", map->ID, map->name.c_str(), map->width, map->height);
	}
}

Controller::~Controller(void)
{
	for(Map *m : maps)
	{
		delete m;
	}
	maps.clear();
}

