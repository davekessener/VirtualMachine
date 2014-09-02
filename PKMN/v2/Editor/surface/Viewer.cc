#include "Viewer.h"
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

	Tileset::bind();
	dav::gl::draw_face2d(0, 0, (double)width() / 1024.0, (double)height() / 1024.0, p.x, p.y, q.x, q.y);
}

}}

