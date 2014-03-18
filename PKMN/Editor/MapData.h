#ifndef EDITOR_MAPDATA_H
#define EDITOR_MAPDATA_H

#include <string>
#include <vector>
#include <cassert>
#include <NBT/NBT.h>
#include "Settings.h"

namespace editor
{
	namespace map
	{
		class Layer
		{
			public:
				Layer(int = 1, int = 1);
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
			void addLayer(int);
			std::vector<map::Layer>& operator[](int);
			nbt::TAG_Compound::ptr_t save( );
			enum
			{
				LAYER_BOTTOM = 0,
				LAYER_INTER = 1,
				LAYER_TOP = 2
			};
		protected:
			virtual void writeToNBT(nbt::TAG_Compound::ptr_t);
		public:
			const std::string name;
			const long ID;
		private:
			int _w, _h;
			std::vector<map::Layer> _bottom, _inter, _top;
		private:
			MapData(const MapData&);
			MapData& operator=(const MapData&);

			std::vector<unsigned int> flatten(std::vector<map::Layer>);
	};
}

#endif

