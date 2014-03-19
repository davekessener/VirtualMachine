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
			std::vector<unsigned int> v(_w * _h);

			for(auto i = _data->begin() ; i != _data->end() ; ++i)
			{
				v.insert(v.end(), (*i)->begin(), (*i)->end());
			}

			return v;
		}
	}

// # ===========================================================================

	MapData::MapData(int id, const std::string& n, int w, int h)
		: ID(id), name(n), _w(w), _h(h), _bottom(1, map::Layer(w, h)), _inter(1, map::Layer(w, h)), _top(1, map::Layer(w, h))
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

	void MapData::addLayer(int idx)
	{
		this->operator[](idx).push_back(map::Layer(_w, _h));
	}

	std::vector<map::Layer>& MapData::operator[](int i)
	{
		assert(i>=0&&i<3);

		return i ? (i == 1 ? _inter : _top) : _bottom;
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

		return map;
	}

	std::vector<unsigned int> MapData::flatten(std::vector<map::Layer> layers)
	{
		std::vector<unsigned int> v;
		
		v.reserve(layers.size() * _w * _h);

		for(auto i = layers.begin() ; i != layers.end() ; ++i)
		{
			std::vector<unsigned int> t(i->flatten());

			v.insert(v.end(), t.begin(), t.end());
		}

		return v;
	}

	void MapData::writeToNBT(nbt::TAG_Compound::ptr_t nbt)
	{
		nbt->setString(Settings::NBT_MAP_NAME, name);
		nbt->setLong(Settings::NBT_MAP_ID, ID);
		nbt->setInt(Settings::NBT_MAP_WIDTH, _w);
		nbt->setInt(Settings::NBT_MAP_HEIGHT, _h);
		
		nbt::TAG_Compound::ptr_t data = nbt::Make<nbt::TAG_Compound>(Settings::NBT_MAP_DATA);
		data->setIntArray(Settings::NBT_MAP_DATA_BOTTOM, flatten(_bottom));
		data->setIntArray(Settings::NBT_MAP_DATA_INTER, flatten(_inter));
		data->setIntArray(Settings::NBT_MAP_DATA_TOP, flatten(_top));

		nbt->setCompoundTag(data);
	}

	void MapData::readFromNBT(nbt::TAG_Compound::ptr_t nbt)
	{
		nbt::TAG_Compound::ptr_t data = nbt->getCompoundTag(Settings::NBT_MAP_DATA);

		auto read = [this](std::vector<unsigned int> d, std::vector<map::Layer> v)
			{
				assert(d.size()%(_w*_h)==0);

				for(auto i = d.cbegin() ; i != d.cend() ; i += _w * _h)
				{
					v.push_back(map::Layer(i, _w, _h));
				}
			};

		read(data->getIntArray(Settings::NBT_MAP_DATA_BOTTOM), _bottom);
		read(data->getIntArray(Settings::NBT_MAP_DATA_INTER), _inter);
		read(data->getIntArray(Settings::NBT_MAP_DATA_TOP), _top);
	}
}

