#include <cassert>
#include "TextStitcher.h"
#include "PNGLoader.h"
#include "GLImageBuffer.h"

#define MXT_TEXTSIZE 256

namespace dav { namespace pkmn { namespace screen {

class TextureStitcher::Stitcher
{
	public:
		Stitcher(uint w) : w_(w), i_(0), c_(MXT_TEXTSIZE / w_), id_(0), next_(nullptr)
			{ img_.resize(MXT_TEXTSIZE * MXT_TEXTSIZE * 4); }
		~Stitcher( ) { delete next_; }
		text_info storeImage(const BYTE *);
	private:
		void copyContent(const BYTE *, uint, uint);
	private:
		uint w_, i_, c_;
		DWORD id_;
		std::vector<BYTE> img_;
		Stitcher *next_;
};

void TextureStitcher::Stitcher::copyContent(const BYTE *p, uint x, uint y)
{
	DWORD *d = reinterpret_cast<DWORD *>(&img_.front());
	const DWORD *s = reinterpret_cast<const DWORD *>(p);

	for(uint dy = 0 ; dy < w_ ; ++dy)
	{
		for(uint dx = 0 ; dx < w_ ; ++dx)
		{
			d[(x + dx) + (y + dy) * MXT_TEXTSIZE] = s[dx + dy * w_];
		}
	}
}

text_info TextureStitcher::Stitcher::storeImage(const BYTE *data)
{
	if(i_ == c_ * c_)
	{
		if(!next_) next_ = new Stitcher(w_);

		return next_->storeImage(data);
	}
	else
	{
		uint x = (i_ % c_) * w_, 
			 y = (i_ / c_) * w_;

		++i_;

		copyContent(data, x, y);

		text_info ti
		{
			id_ = GLImageBuffer::UpdateImage(id_, &img_.front(), MXT_TEXTSIZE),
			(float)(x / (double)MXT_TEXTSIZE),
			(float)(y / (double)MXT_TEXTSIZE),
			(float)((x + w_) / (double)MXT_TEXTSIZE),
			(float)((y + w_) / (double)MXT_TEXTSIZE),
			w_
		};

//		std::cout << "TI: " << ti.id << ", [" << ti.u1 << ", " << ti.v1 << "], [" << ti.u2 << ", " << ti.v2 << "], " << ti.size << std::endl;

		return ti;
	}
}

// # ===========================================================================

text_info TextureStitcher::doLoadIcon(const std::string& s)
{
	if(s.empty()) return text_info{0, 0, 0, 0, 0, 0};

	auto i(lookup_.find(s));

	if(i == lookup_.end())
	{
		std::vector<BYTE> buf;
		auto info = PNGLoader::LoadRawPNG(s, buf);
		DWORD w = info.s;

		assert(info.w==info.h&&info.w==info.s);

		auto j(st_.find(w));

		if(j == st_.end())
		{
			j = st_.insert(std::make_pair(w, new Stitcher(w))).first;
		}

		i = lookup_.insert(std::make_pair(s, std::make_pair(j->second->storeImage(&buf.front()), w))).first;
	}

	return i->second.first;
}

TextureStitcher::~TextureStitcher(void)
{
	for(auto& p : st_)
	{
		delete p.second;
	}
}

}}}

