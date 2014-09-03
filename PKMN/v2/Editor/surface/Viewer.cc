#include "Viewer.h"
#include "../Controller.h"
#include "../Tileset.h"
#include <dav/gl.h>

namespace editor { namespace surface {

void Viewer::i_doInit(void)
{
}

void Viewer::i_doUpdate(int d)
{
}

void Viewer::i_doPrerender(void)
{
}

void Viewer::i_doRender(void) const
{
	point p(getAbsCoords()), q(p.x + width(), p.y + height());

	::editor::Tileset::bind();

	const auto &m(Controller::getMap());

	for(int i = 0 ; i < 4 ; ++i)
	{
		const auto &v(m[i]);

		for(int y = 0, Y = (height() + 15) / 16 ; y < Y ; ++y)
		{
			for(int x = 0, X = (width() + 15) / 16 ; x < X ; ++x)
			{
				if(x >= (int)m.width() || y >= (int)m.height()) continue;

				DWORD id = v.at(x + y * m.width());

				if(!id) continue;

				int dx = (id % 64) * 16, dy = (id / 64) * 16;
#define min(a,b) (((long)(a)<(long)(b))?(a):(b))
				int w = min(width(), (x + 1) * 16) - x * 16,
					h = min(height(), (y + 1) * 16) - y * 16;
#undef min
				dav::gl::draw_face2d(dx / 1024.0, dy / 1024.0, (dx + w) / 1024.0, (dy + h) / 1024.0,
									 p.x + x * 16, p.y + y * 16, p.x + x * 16 + w, p.y + y * 16 + h);
			}
		}
	}
}

point Viewer::getScrollSizes(void) const
{
	return point(0, 0);
}

void Viewer::setScrollPos(int dx, int dy)
{
}

}}

