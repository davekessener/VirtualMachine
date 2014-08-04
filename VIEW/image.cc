#include <gz/gzstream>
#include <vector>
#include <cassert>
#include "image.h"
//#include "gl.h"
#include "dav_header.h"
#include "Manager.h"
#include "encrypt_stream.hpp"

void Image::load(const std::string& fn)
{
	dav::imgheader_t dav;
	gzip::igzstream in_raw(fn.c_str());
	encrypt_stream<gzip::igzstream> in(in_raw);

	in.read(reinterpret_cast<char *>(&dav), sizeof(dav));
	assert(dav.id==dav::DAV_MAGIC);
	width_ = dav.width;
	height_ = dav.height;
	data_.resize(width_ * height_ * 3);
	assert(data_.size()+sizeof(dav)==dav.filesize);
	in.read(reinterpret_cast<char *>(&*data_.begin()), data_.size());
	in.close();
}

void Image::create(void)
{
	assert(data_.size()==(size_t)(width_*height_*3));
//	id_ = docreate(&*data_.cbegin(), width_, height_, &pw_, &ph_);
	Manager::instance().createTexture(&*data_.cbegin(), width_, height_);
	std::vector<BYTE>().swap(data_);
	loaded_ = true;
}

//void Image::bind(void) const
//{
//	gl::bind_texture(id_);
//}

void Image::erase(void)
{
//	gl::bind_texture(0);
//	gl::delete_texture(id_);
//	id_ = 0;
	std::vector<BYTE>().swap(data_);
	loaded_ = false;
}

void Image::draw(int w, int h, int dx, int dy) const
{
	int x1((w - width_) / 2 + dx), y1((h - height_) / 2 + dy);
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

//	float cx(width_ / (float)pw_), cy(height_ / (float)ph_);
//
//	u1 *= cx; u2 *= cx;
//	v1 *= cy; v2 *= cy;

//	gl::draw_face2d(u1, v1, u2, v2, x1, y1, x2, y2);
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

//	gl::draw_face2d(u1, v1, u2, v2, x1, y1, x2, y2);
	Manager::instance().draw(u1, v1, u2, v2, x1, y1, x2, y2);
}

//DWORD Image::docreate(const BYTE *img, int w, int h, int *ppw, int *pph)
//{
//	int pw = 1, ph = 1;
//
//	while(pw < w) pw <<= 1;
//	while(ph < h) ph <<= 1;
//	
//	std::vector<BYTE> buf(pw * ph * 3);
//	
//	for(int y = 0 ; y < h ; ++y)
//	{
//		for(int x = 0 ; x < w ; ++x)
//		{
//			buf[x * 3 + 0 + y * pw * 3] = img[x * 3 + 0 + y * w * 3];
//			buf[x * 3 + 1 + y * pw * 3] = img[x * 3 + 1 + y * w * 3];
//			buf[x * 3 + 2 + y * pw * 3] = img[x * 3 + 2 + y * w * 3];
//		}
//	}
//
//	if(ppw) *ppw = pw;
//	if(pph) *pph = ph;
//
//	return gl::create_texture(&*buf.cbegin(), pw, ph);
//}

