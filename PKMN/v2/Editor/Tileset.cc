#include "Tileset.h"
#include <dav/gl.h>
#include "surface/PNGLoader.h"

#define MXT_TILESETPATH "tileset.png"

bool editor::Tileset::GRID = false;

namespace editor
{
	Tileset::Tileset(void) : id_(surface::PNGLoader::loadPNG(MXT_TILESETPATH)), cur_(0, 0)
	{
	}

	void Tileset::doBind(void) const
	{
		dav::gl::bind_texture(id_);
	}

	void Tileset::doSet(const TileBlock& tb)
	{
		cur_ = tb;
	}

	TileBlock Tileset::doGet(void) const
	{
		return cur_;
	}
}

