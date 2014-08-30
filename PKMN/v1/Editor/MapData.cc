#include "MapData.h"

namespace editor
{
	namespace map
	{
		Layer::Layer(int w, int h) : _w(w), _h(h), _data(new std::vector<std::shared_ptr<std::vector<int>>>(_w))
		{
			assert(_w>0&&_h>0);

			for(auto i = _data->begin() ; i != _data->end() ; ++i)
			{
				i->reset(new std::vector<int>(_h, 0));
			}
		}

		Layer::Layer(std::vector<unsigned int>::const_iterator i1, int w, int h)
			: _w(w), _h(h), _data(new std::vector<std::shared_ptr<std::vector<int>>>(w))
		{
			assert(_w>0&&_h>0);

			for(auto i = _data->begin() ; i != _data->end() ; ++i)
			{
				i->reset(new std::vector<int>(i1, i1 + h));
				i1 += h;
			}
		}

		Layer::~Layer(void)
		{
		}

		std::vector<int>& Layer::operator[](int i)
		{
			return *_data->at(i);
		}

		void Layer::resize(int w, int h)
		{
			assert(w>0&&h>0);

			if(h != _h)
			{
				_h = h;

				for(auto i = _data->begin() ; i != _data->end() ; ++i)
				{
					(*i)->resize(_h);
				}
			}

			if(w != _w)
			{
				_data->reserve(w);

				if(w > _w)
				{
					int d = w - _w;
					for(auto i = _data->end() ; d ; --d)
					{
						(--i)->reset(new std::vector<int>(_h, 0));
					}
				}
				else
				{
					_data->resize(w);
				}

				_w = w;
			}
		}

		int Layer::width(void) const
		{
			return _w;
		}

		int Layer::height(void) const
		{
			return _h;
		}

		std::vector<unsigned int> Layer::flatten(void) const
		{
			std::vector<unsigned int> v;

			v.resize(_w * _h);
			auto i = v.begin();

			for(size_t y = 0 ; y < _h ; ++y)
			{
				for(size_t x = 0 ; x < _w ; ++x)
				{
					*i = _data->at(x)->at(y);
					++i;
				}
			}

//			for(auto i = _data->begin() ; i != _data->end() ; ++i)
//			{
//				v.insert(v.end(), (*i)->begin(), (*i)->end());
//			}

			return v;
		}
	}

// # ===========================================================================

	MapData::MapData(int id, const std::string& n, int w, int h)
		: ID(id), name(n), _w(w), _h(h), _layers(LAYERS, map::Layer(w, h))
	{
	}

	MapData::~MapData(void)
	{
	}

	int MapData::width(void) const
	{
		return _w;
	}

	int MapData::height(void) const
	{
		return _h;
	}

	map::Layer MapData::operator[](int i)
	{
		return _layers.at(i);
	}

	nbt::TAG_Compound::ptr_t MapData::save(void)
	{
		nbt::TAG_Compound::ptr_t nbt = nbt::Make<nbt::TAG_Compound>();

		writeToNBT(nbt);

		return nbt;
	}

	MapData *MapData::load(nbt::TAG_Compound::ptr_t nbt)
	{
		MapData *map = new MapData(	nbt->getLong(Settings::NBT_MAP_ID),
									nbt->getString(Settings::NBT_MAP_NAME),
									nbt->getInt(Settings::NBT_MAP_WIDTH),
									nbt->getInt(Settings::NBT_MAP_HEIGHT));

		map->readFromNBT(nbt);

		LOG("Read map '%s'(%d) %d x %d", map->name.c_str(), map->ID, map->_w, map->_h);

		return map;
	}

	void MapData::writeToNBT(nbt::TAG_Compound::ptr_t nbt)
	{
		nbt->setString(Settings::NBT_MAP_NAME, name);
		nbt->setLong(Settings::NBT_MAP_ID, ID);
		nbt->setInt(Settings::NBT_MAP_WIDTH, _w);
		nbt->setInt(Settings::NBT_MAP_HEIGHT, _h);
		
		nbt::TAG_Compound::ptr_t data = nbt::Make<nbt::TAG_Compound>(Settings::NBT_MAP_DATA);
		data->setIntArray(Settings::NBT_MAP_DATA_BOTTOM, _layers.at(LAYER_BOTTOM1).flatten());
		data->setIntArray(Settings::NBT_MAP_DATA_INTER, _layers.at(LAYER_BOTTOM2).flatten());
		data->setIntArray(Settings::NBT_MAP_DATA_ANIMATION, _layers.at(LAYER_INTER).flatten());
		data->setIntArray(Settings::NBT_MAP_DATA_TOP, _layers.at(LAYER_TOP).flatten());

		nbt->setCompoundTag(data);
	}

	void MapData::readFromNBT(nbt::TAG_Compound::ptr_t nbt)
	{
		nbt::TAG_Compound::ptr_t data = nbt->getCompoundTag(Settings::NBT_MAP_DATA);

		auto read = [this, data](const std::string& name, std::vector<map::Layer>& v)
			{
				v.clear();

				if(data->hasTag(name))
				{
					std::vector<unsigned int> d = data->getIntArray(name);

					assert(d.size()%(_w*_h)==0);

					for(auto i = d.cbegin() ; i != d.cend() ; i += _w * _h)
					{
						v.push_back(map::Layer(i, _w, _h));
					}
				}
				else
				{
					LOG("Map '%s'(%d) cannot load layer '%s', will use default.", this->name.c_str(), ID, name.c_str());
				}
			};

		_layers.clear();

		std::vector<map::Layer> v;

		read(Settings::NBT_MAP_DATA_BOTTOM, v);

		_layers.push_back(v.size() == 0 ? map::Layer(_w, _h) : v.at(0));
		_layers.push_back(v.size() <= 1 ? map::Layer(_w, _h) : v.at(1));

		read(Settings::NBT_MAP_DATA_INTER, v);

		_layers.push_back(v.empty() ? map::Layer(_w, _h) : v.at(0));

		read(Settings::NBT_MAP_DATA_TOP, v);

		_layers.push_back(v.empty() ? map::Layer(_w, _h) : v.at(0));
	}
}

