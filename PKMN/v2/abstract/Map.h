#ifndef PKMN_ABSTRACT_MAP_H
#define PKMN_ABSTRACT_MAP_H

#include "../common.h"
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
			typedef std::vector<DWORD> vec_t;

			public:
				Map(nbt::TAG_Compound::ptr_t);
				Map(QWORD, const std::string&, uint, uint);
				const std::string& name( ) const { return name_; }
				QWORD ID( ) const { return id_; }
				void setID(QWORD id) { id_ = id; changed_ = true; }
				uint width( ) const { return width_; }
				uint height( ) const { return height_; }
				bool isLayerEmpty(size_t idx) const { assert(idx<MXT_LC); return empty_[idx]; }
				static constexpr uint size( ) { return MXT_LC; }
				const vec_t& operator[](size_t idx) const { assert(idx<MXT_LC); return layers_[idx]; }
				inline DWORD get(uint l, uint x, uint y) const { return (*this)[l].at(x + y * width_); }
				inline void set(uint l, uint x, uint y, DWORD v)
					{ assert(l<MXT_LC); layers_[l].at(x + y * width_) = v; changed_ = true; }
				inline bool hasChanged( ) const { return changed_; }
				inline void setName(const std::string& s) { name_ = s; changed_ = true; }
				nbt::TAG_Compound::ptr_t save( ) const;
			private:
				QWORD id_;
				std::string name_;
				vec_t layers_[MXT_LC];
				bool empty_[MXT_LC];
				uint width_, height_;
				mutable bool changed_;
		};
	}
}

#endif

