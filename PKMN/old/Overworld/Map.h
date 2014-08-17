#ifndef MAP_H
#define MAP_H

#include <string>
#include <vector>
#include <cstdint>

class Map
{
	public:
		Map(long _id, const std::string& _name, int _w, int _h) : name(_name), ID(_id), width(_w), height(_h) { }
		~Map( ) { }
		void setBottomLayer(std::vector<std::uint32_t>&& v) { bottom = v; }
		void setAnimationLayer(std::vector<std::uint32_t>&& v) { animation = v; }
		void setTopLayer(std::vector<std::uint32_t>&& v) { top = v; }
		const std::vector<std::uint32_t>& getBottomLayer( ) const { return bottom; }
		const std::vector<std::uint32_t>& getAnimationLayer( ) const { return animation; }
		const std::vector<std::uint32_t>& getTopLayer( ) const { return top; }

		const std::string name;
		const int width, height;
		const long ID;
	private:
		Map& operator=(const Map&);

		std::vector<std::uint32_t> bottom, animation, top;
};

#endif

