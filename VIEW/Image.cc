#include <dav/gzstream.h>
#include <vector>
#include <cassert>
#include "Image.h"
#include "dav_header.h"
#include "Manager.h"
#include "encrypt_stream.hpp"

using namespace dav;

void Image::load(const std::string& fn)
{
	imgheader_t dav;
	gzip::igzstream in_raw(fn.c_str());
	encrypt_stream<gzip::igzstream> in(in_raw);

	in.read(reinterpret_cast<char *>(&dav), sizeof(dav));
	assert(dav.id==DAV_MAGIC);
	width_ = dav.width;
	height_ = dav.height;
	data_.resize(width_ * height_ * 3);
	assert(data_.size()+sizeof(dav)==dav.filesize);
	in.read(reinterpret_cast<char *>(&*data_.begin()), data_.size());
	in.close();

	assert(data_.size()==(size_t)(width_*height_*3));
	Manager::instance().createTexture(&*data_.cbegin(), width_, height_);
	std::vector<BYTE>().swap(data_);
	loaded_ = true;
}

void Image::erase(void)
{
	std::vector<BYTE>().swap(data_);
	loaded_ = false;
}

void Image::draw(int w, int h, int dx, int dy) const
{
#define min(a,b) (((a)<(b))?(a):(b))
	int tx(-min(0, (w - width_) / 2)), ty(-min(0, (h - height_) / 2));
#undef min
	int x1((w - width_) / 2 + tx + dx), y1((h - height_) / 2 + ty + dy);
	int x2(x1 + width_), y2(y1 + height_);
	float u1(0.0), v1(0.0), u2(1.0), v2(1.0);

	if(x1 < 0)
	{
		u1 = -x1 / (float)width_;
		x1 = 0;
	}

	if(x2 > w)
	{
		u2 = 1.0 - (x2 - w) / (float)width_;
		x2 = w;
	}

	if(y1 < 0)
	{
		v1 = -y1 / (float)height_;
		y1 = 0;
	}

	if(y2 > h)
	{
		v2 = 1.0 - (y2 - h) / (float)height_;
		y2 = h;
	}

	Manager::instance().draw(u1, v1, u2, v2, x1, y1, x2, y2);
}

void Image::center(int w, int h) const
{
	float r1(width_ / (float)height_), r2(w / (float)h);
	float u1(0.0), v1(0.0), u2(1.0), v2(1.0);
	int x1(0), y1(0), x2(w), y2(h);

	if(r1 == r2)
	{
	}
	else if(r1 > r2)
	{
		int th = static_cast<int>(w / r1);
		y1 = (h - th) / 2;
		y2 = y1 + th;
	}
	else if(r1 < r2)
	{
		int tw = static_cast<int>(h * r1);
		x1 = (w - tw) / 2;
		x2 = x1 + tw;
	}

	Manager::instance().draw(u1, v1, u2, v2, x1, y1, x2, y2);
}

