#include <SDL2/SDL_opengl.h>
#include <cassert>
#include "Tessellator.h"

#define MXT_STRIDE 5

Tessellator& Tessellator::instance(void)
{
	static Tessellator tex;
	return tex;
}

Tessellator::Tessellator(void) : drawing_(false), dx_(0.0), dy_(0.0), dz_(0.0)
{
}

Tessellator::~Tessellator(void) noexcept
{
}

void Tessellator::startDrawing(void)
{
	assert(!drawing_);

	drawing_ = true;
	reset();
	hasTextures_ = false;
}

void Tessellator::draw(void)
{
	assert(drawing_);

	drawing_ = false;

	if(count_ > 0)
	{
		if(hasTextures_)
		{
			glTexCoordPointer(2, GL_FLOAT, MXT_STRIDE * sizeof(data_t), &*buf_.begin() + 3);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		}

		glVertexPointer(3, GL_FLOAT, MXT_STRIDE * sizeof(data_t), &*buf_.begin());

		glEnableClientState(GL_VERTEX_ARRAY);

		glDrawArrays(GL_QUADS, 0, count_);

		glDisableClientState(GL_VERTEX_ARRAY);

		if(hasTextures_)
		{
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		}
	}

	reset();
}

void Tessellator::reset(void)
{
	count_ = 0;
	idx_ = 0;
	setOffset(0.0, 0.0, 0.0);
}

void Tessellator::addVertex(float x, float y, float z)
{
	++count_;

	if(hasTextures_)
	{
		buf_[idx_ + 3].f = u_;
		buf_[idx_ + 4].f = v_;
	}

	buf_[idx_ + 0].f = x + dx_;
	buf_[idx_ + 1].f = y + dy_;
	buf_[idx_ + 2].f = z + dz_;

	idx_ += MXT_STRIDE;

	if(count_ % 4 == 0 && (size_t)idx_ >= MXT_TESS_BUFSIZE - MXT_STRIDE * sizeof(data_t))
	{
		draw();
		drawing_ = true;
	}
}

void Tessellator::addUV(float u, float v)
{
	hasTextures_ = true;
	u_ = u;
	v_ = v;
}

