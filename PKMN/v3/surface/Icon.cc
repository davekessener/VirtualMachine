#include "Icon.h"
#include "TextStitcher.h"

namespace dav { namespace pkmn { namespace screen {

void Icon::load(const std::string& s)
{
	if(path_ != s)
	{
		path_ = s;
		img_.id = 0;
		dirty(true);
	}
}

void Icon::i_doPrerender(void)
{
	if(!img_.id)
	{
		img_ = TextureStitcher::LoadIcon(path_);
	}
}

void Icon::i_doRender(void)
{
	if(img_.id)
	{
		int x = (width() - img_.size) / 2,
			y = (height() - img_.size) / 2;

		draw(img_.id, img_.u1, img_.v1, img_.u2, img_.v2, x, y, x + img_.size, y + img_.size);
	}
}

}}}

