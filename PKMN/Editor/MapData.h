#ifndef EDITOR_MAPDATA_H
#define EDITOR_MAPDATA_H

#include <string>
#include <vector>
#include <cassert>
#include <nbt/NBT.h>
#include <Misc/Logger.h>
#include "Settings.h"

namespace editor
{
	namespace map
	{
		class Layer
		{
			public:
				Layer(int = 1, int = 1);
				Layer(std::vector<unsigned int>::const_iterator, int, int);
				~Layer( );
				std::vector<int>& operator[](int);
				void resize(int, int);
				int width( ) const;
				int height( ) const;
				std::vector<unsigned int> flatten( ) const;
			private:
				int _w, _h;
				std::shared_ptr<std::vector<std::shared_ptr<std::vector<int>>>> _data;
		};
	}

	class MapData
	{
		public:
			MapData(int, const std::string&, int, int);
			~MapData( );
			int width( ) const;
			int height( ) const;
			map::Layer operator[](int);
			nbt::TAG_Compound::ptr_t save( );
			static MapData *load(nbt::TAG_Compound::ptr_t);
			enum
			{
				LAYER_BOTTOM1 = 0,
				LAYER_BOTTOM2 = 1,
				LAYER_INTER = 2,
				LAYER_TOP = 3,
				LAYERS = 4
			};
		protected:
			void writeToNBT(nbt::TAG_Compound::ptr_t);
			void readFromNBT(nbt::TAG_Compound::ptr_t);
		public:
			const std::string name;
			const long ID;
		private:
			int _w, _h;
			std::vector<map::Layer> _layers;
		private:
			MapData(const MapData&);
			MapData& operator=(const MapData&);
	};
}

#endif

