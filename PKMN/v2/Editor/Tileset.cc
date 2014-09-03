#include "Tileset.h"
#include <dav/gl.h>
#include "surface/PNGLoader.h"

#define MXT_TILESETPATH "tileset.png"

namespace editor
{
	Tileset::Tileset(void) : id_(surface::PNGLoader::loadPNG(MXT_TILESETPATH))
	{
	}

	void Tileset::doBind(void) const
	{
		dav::gl::bind_texture(id_);
	}
}

