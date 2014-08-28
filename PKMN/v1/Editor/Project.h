#ifndef EDITOR_PROJECT_H
#define EDITOR_PROJECT_H

#include <cstdint>
#include "../Surface/Geometry.h"
#include "MapData.h"

namespace editor
{
	class Project
	{
		public:
		typedef std::uint64_t map_id;
		typedef std::shared_ptr<MapData> map_ptr;

		public:
			Project(const std::string&);
			~Project( );
			map_id createMap(const std::string&, int, int);
			map_ptr getMap(map_id);
			void save( );
		private:
			map_id _nextID;
			std::string _fn;
			std::vector<std::shared_ptr<MapData>> _maps;
		private:
			Project(const Project&);
			Project& operator=(const Project&);
			void load( );
	};
}

#endif

