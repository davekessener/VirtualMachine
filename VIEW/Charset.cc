#include <vector>
#include <SDL2/SDL_opengl.h>
#include "Charset.h"
#include "inc.h"
#include "gl.h"
#include "png.h"

struct Charset::Impl
{
	DWORD id_;
	int w_, h_;
	int s_;
};

Charset::Charset(void) : impl_(new Impl)
{
	impl_->id_ = 0;
	impl_->s_ = 16;
}

Charset::~Charset(void)
{
	gl::delete_texture(impl_->id_);

	delete impl_;
}

void Charset::load(const std::string& fn)
{
	std::vector<BYTE> data;

	load_png(data, fn, &impl_->w_, &impl_->h_);

	glGenTextures(1, &impl_->id_);
	glBindTexture(GL_TEXTURE_2D, impl_->id_);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, impl_->w_, impl_->h_, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, &*data.cbegin());
}

void Charset::renderStringAt(const std::string& s, int x, int y, int c) const
{
	for(const char &ch : s)
	{
		renderCharAt(ch, x, y, c);
		x += impl_->s_;
	}
}

void Charset::renderCharAt(char ch, int x, int y, int c) const
{
	gl::bind_texture(impl_->id_);

	float u = (ch % 16) / 16.0, v = (ch / 16) / 16.0;

	if(c != 0xffffff)
	{
#define SF(i) (((c>>(i))&0xff)/255.0)
	glColor3f(SF(16), SF(8), SF(0));
#undef SF
	}

	gl::draw_face2d(u, v, u + 0.0625, v + 0.0625, x, y, x + impl_->s_, y + impl_->s_);
	
	if(c != 0xffffff)
	{
		glColor3f(1.0, 1.0, 1.0);
	}
}

int Charset::getStringWidth(const std::string& s) const
{
	return impl_->s_ * s.length();
}

int Charset::getCharHeight(void) const
{
	return impl_->s_;
}

void Charset::setCharSize(int s)
{
	impl_->s_ = s;
}

