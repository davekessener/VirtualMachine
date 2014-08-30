#ifndef PKMN_ABSTRACT_MAP_H
#define PKMN_ABSTRACT_MAP_H

#include <string>
#include <vector>
#include <cassert>
#include <nbt/NBT.h>

#define MXT_LC 4

namespace pkmn
{
	namespace abstract
	{
		class Map
		{
			typedef unsigned int uint;
			typedef std::vector<uint> vec_t;

			public:
				Map(nbt::TAG_Compound::ptr_t);
				const std::string& name( ) const { return name_; }
				size_t ID( ) const { return id_; }
				uint width( ) const { return width_; }
				uint height( ) const { return height_; }
				bool isLayerEmpty(size_t idx) const { assert(idx<MXT_LC); return empty_[idx]; }
				static constexpr uint size( ) { return MXT_LC; }
				const vec_t& operator[](size_t idx) const { assert(idx<MXT_LC); return layers_[idx]; }
			private:
				std::string name_;
				vec_t layers_[MXT_LC];
				bool empty_[MXT_LC];
				uint width_, height_;
				size_t id_;
		};
	}
}

#endif

