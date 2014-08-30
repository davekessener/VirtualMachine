#include "TileSet.h"
#include "../Config.h"
#include <dav/gl.h>
#include <dav/Logger.h>

namespace pkmn { namespace render {

void TileSet::bind(void) const
{
	dav::gl::bind_texture(id_);
}

void TileSet::render(int x, int y, DWORD t) const
{
	DWORD d = Config::TILE_SIZE;
	
	if(x <= -(int)d || y <= -(int)d || x >= (long)Config::SCREEN_WIDTH || y >= (long)Config::SCREEN_HEIGHT) return;

	DWORD tt(tbl_.at(t));
	int c = ds_ / d;

	double u = (tt % c) / (double)c;
	double v = (tt / c) / (double)c;
	double duv = 1 / (double)c;

//	LOG("Rendering %u: %lf, %lf, %lf, %lf, %d, %d, %d, %d", id_, u, v, u + duv, v + duv, x, y, x + d, y + d);

	dav::gl::draw_face2d(u, v, u + duv, v + duv, x, y, (x + d), (y + d));
}

}}

