#include "ButtonHelper.h"
#include "PNGLoader.h"
#include <dav/gl.h>

#define MXT_BTN1PATH "icons/btn_1.png"
#define MXT_BTN2PATH "icons/btn_2.png"

namespace editor { namespace surface {

void ButtonHelper::renderButton(int x1, int y1, int x2, int y2, bool down)
{
	using dav::gl::draw_face2d;

	if(x1 < 0 || x2 < 0 || y1 < 0 || y2 < 0 || x2 - x1 < 4 || y2 - y1 < 4) return;

	dav::gl::bind_texture(down ? instance().id2_ : instance().id1_);

	if(x2 - x1 == 16 && y2 - y1 == 16)
	{
		draw_face2d(0, 0, 1, 1, x1, y1, x2, y2);
	}
	else
	{
		draw_face2d(0, 0, 0.125, 0.125, x1, y1, x1 + 2, y1 + 2);
		draw_face2d(0.875, 0, 1, 0.125, x2 - 2, y1, x2, y1 + 2);
		draw_face2d(0, 0.875, 0.125, 1, x1, y2 - 2, x1 + 2, y2);
		draw_face2d(0.875, 0.875, 1, 1, x2 - 2, y2 - 2, x2, y2);

		bool dox(x2 - x1 > 4), doy(y2 - y1 > 4);

		if(dox)
		{
			draw_face2d(0.125, 0, 0.875, 0.125, x1 + 2, y1, x2 - 2, y1 + 2);
			draw_face2d(0.125, 0.875, 0.875, 1, x1 + 2, y2 - 2, x2 - 2, y2);
		}

		if(doy)
		{
			draw_face2d(0, 0.125, 0.125, 0.875, x1, y1 + 2, x1 + 2, y2 - 2);
			draw_face2d(0.875, 0.125, 1, 0.875, x2 - 2, y1 + 2, x2, y2 - 2);
		}

		if(dox && doy)
		{
			draw_face2d(0.125, 0.125, 0.875, 0.875, x1 + 2, y1 + 2, x2 - 2, y2 - 2);
		}
	}
}

ButtonHelper::ButtonHelper(void)
	: id1_(PNGLoader::loadPNG(MXT_BTN1PATH)),
	  id2_(PNGLoader::loadPNG(MXT_BTN2PATH))
{
}

}}

