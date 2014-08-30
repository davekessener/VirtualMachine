#include <cassert>
#include "TSHelper.h"
#include "GLImageBuffer.h"
#include "../Config.h"
#include <dav/Logger.h>
#include <png/png.hpp>

namespace pkmn { namespace render {

struct TSHelper::Impl
{
	typedef png::image<png::rgba_pixel> img_t;

	img_t img_;
	std::set<DWORD> ids_;
};

TileSet TSHelper::generate(const std::set<DWORD>& data)
{
	size_t c = data.size();
	int d = Config::TILE_SIZE, d2 = d * d;
	size_t ds = d;

	assert(d>0);

	LOG("Generating TileSet [...]");

	while(ds * ds < c * d2) ds *= 2;;

	LOG("> %lu x %lu tiles (%d x %d)", ds / d, ds / d, d, d);

	std::vector<BYTE> buf(ds * ds * 4);
	std::map<DWORD, DWORD> tsm;

	Impl::img_t &img(instance().impl_->img_);

	size_t w = img.get_width()  / d,
		   h = img.get_height() / d,
		   s = ds / d;

	LOG("> TileSet is sized %lu x %lu tiles", w, h);

	assert(w==h);

	DWORD i = 0;
	for(const DWORD& j : data)
	{
		tsm[j] = i;

		LOG("> Mapped %u -> %u", j, i);

		int sx = (j % w) * d, sy = (j / w) * d;
		int tx = (i % s) * d, ty = (i / s) * d;

		LOG("> Tile %u @%d,%d -> @%d,%d", i, sx, sy, tx, ty);

		for(int dy = 0 ; dy < d ; ++dy)
		{
			for(int dx = 0 ; dx < d ; ++dx)
			{
				auto cc = img.get_pixel(sx + dx, sy + dy);
				buf[(dx + tx + (dy + ty) * ds) * 4 + 0] = cc.red;
				buf[(dx + tx + (dy + ty) * ds) * 4 + 1] = cc.green;
				buf[(dx + tx + (dy + ty) * ds) * 4 + 2] = cc.blue;
				buf[(dx + tx + (dy + ty) * ds) * 4 + 3] = cc.alpha;
			}
		}

		++i;
	}

	LOG("> Total of %u tiles.", i);

	DWORD id = GLImageBuffer::getImage(&*buf.cbegin(), ds);

	instance().impl_->ids_.insert(id);

	LOG("> ID: %u, DS: %lu", id, ds);
	LOG("[DONE]");

	return TileSet(id, ds, std::move(tsm));
}

TSHelper::TSHelper(void) : impl_(new Impl)
{
	impl_->img_.read(Config::TILESET_PATH);
}

TSHelper::~TSHelper(void)
{
	delete impl_;
}

}}

