#include "Controller.h"
#include <nbt/NBT.h>
#include <cassert>
#include "MapRenderer.h"
#include "OWSpriteRenderer.h"

Controller *Controller::_instance = NULL;

void Controller::create(const std::string& path)
{
	assert(!_instance);

	_instance = new Controller(path);
}

void Controller::destroy(void)
{
	assert(_instance);

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
	Map *cMap = maps.at(curMap);
	int dx = camera.getHDist(), dy = camera.getVDist();
	MapRenderer::renderMap(*cMap, ticks, dx, dy);
	OWSpriteRenderer::renderSpriteWithOffset(thePlayer, dx, dy);
}

void Controller::update(void)
{
	++ticks;
	thePlayer.update();
	camera.update();
}

bool Controller::input(int in)
{
	switch(in)
	{
		case Controls::UP:
			thePlayer.walk(Direction::UP);
			break;
		case Controls::DOWN:
			thePlayer.walk(Direction::DOWN);
			break;
		case Controls::LEFT:
			thePlayer.walk(Direction::LEFT);
			break;
		case Controls::RIGHT:
			thePlayer.walk(Direction::RIGHT);
			break;
	}

	return false;
}

// # ===========================================================================

Controller::Controller(const std::string& _saveFn) : saveFn(_saveFn), curMap(0), ticks(0)
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

	camera.lock(&thePlayer);
}

Controller::~Controller(void)
{
	for(Map *m : maps)
	{
		delete m;
	}
	maps.clear();
}

