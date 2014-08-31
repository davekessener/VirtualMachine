#include "SHelper.h"
#include <cassert>
#include <boost/filesystem.hpp>
#include <png/png.hpp>
#include <dav/gl.h>
#include <dav/Logger.h>
#include <aux>
#include "../Config.h"
#include "GLImageBuffer.h"

#define MXT_IMGS 512

namespace pkmn { namespace render {

void SHelper::doRender(DWORD id, uint p, int x, int y) const
{
	if(!isRegistered(id))
	{
		LOG("ERR: Trying to render unregistered id %u.", id);
		throw std::string("trying to render unregistered id!");
	}

	dav::gl::bind_texture(id_);

	if(	   x <= -16 || x >= (int)Config::SCREEN_WIDTH
		|| y <= -24 || y >= (int)Config::SCREEN_HEIGHT) return;

	float u1 = p << 4, v1 = idmap_.at(id) * 24;
	float u2 = u1 + 16, v2 = v1 + 24;

	u1 /= MXT_IMGS; v1 /= MXT_IMGS;
	u2 /= MXT_IMGS; v2 /= MXT_IMGS;

//	LOG("Ready to draw sprite @(%d,%d -> %d,%d)[%f,%f -> %f, %f]", x, y, x + 16, y + 24, u1, v1, u2, v2);

	dav::gl::draw_face2d(u1, v1, u2, v2, x, y, x + 16, y + 24);
}

void SHelper::doRegisterSprite(DWORD id)
{
	using dav::aux::lexical_cast;

	if(isRegistered(id))
	{
		LOG("ERR: Sprite %u is already registered.", id);
		throw std::string("sprite is already registered!");
	}

	idmap_[id] = next_;
	BYTE *p = &*getNext(next_);
	next_ = *getNext(next_);

	std::string fn(Config::SPRITE_PATH + lexical_cast<std::string>(id) + ".png");

	if(!boost::filesystem::is_regular_file(fn))
	{
		LOG("ERR: File '%s' is not a file. Image cannot be loaded.");
		throw std::string("image doesn't exist!");
	}

	{
		png::image<png::rgba_pixel> img(fn);
		uint w = img.get_width(), h = img.get_height();

		assert(w<=MXT_IMGS&&h==24);

		for(uint y = 0 ; y < h ; ++y)
		{
			for(uint x = 0 ; x < w ; ++x)
			{
				auto c(img.get_pixel(x, y));
				p[(x + y * MXT_IMGS) * 4 + 0] = c.red;
				p[(x + y * MXT_IMGS) * 4 + 1] = c.green;
				p[(x + y * MXT_IMGS) * 4 + 2] = c.blue;
				p[(x + y * MXT_IMGS) * 4 + 3] = c.alpha;
			}
		}
	}

	if(id_)
	{
		GLImageBuffer::updateImage(id_, &img_.front(), MXT_IMGS);
	}
	else
	{
		id_ = GLImageBuffer::getImage(&img_.front(), MXT_IMGS);
	}
}

void SHelper::doFreeSprite(DWORD id)
{
	auto pi = idmap_.find(id);
	if(pi != idmap_.end())
	{
		LOG("ERR: Cannot free id %u, it is not registered.", id);
		throw std::string("cannot free id!");
	}

	BYTE p = pi->second;
	auto i = getNext(next_), j = getNext(*i);

	if(*i > p)
	{
		*getNext(p) = *i;
		next_ = p;
	}
	else
	{
		while(*j < p) { i = j; j = getNext(*i); }
		*getNext(p) = *i;
		*i = p;
	}
}

SHelper::SHelper(void) : img_(MXT_IMGS * MXT_IMGS * 4, 0xff), id_(0), next_(0)
{
	BYTE *p = &img_.front(), *e = &img_.back();
	BYTE i = 0;

	while(p < e)
	{
		*p = ++i;
		p += 4;
	}
}

SHelper::~SHelper(void) noexcept
{
}

}}

