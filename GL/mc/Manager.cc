#include <array>
#include <memory>
#include <cassert>
#include "Manager.h"
#include <dav/gl.h>
#include "simple_bmp.h"
#include "Block.h"
#include "BlockGrass.h"

#define MXT_BLOCKCOUNT 3

using namespace dav;

Manager& Manager::instance(void)
{
	static Manager m;
	return m;
}

struct Manager::Impl
{
	Impl( );
	std::array<std::shared_ptr<Block>, MXT_BLOCKCOUNT> blocks_;
	std::vector<img_t> imgs_;
};

Manager::Impl::Impl(void)
{
	blocks_[0].reset(new BlockGrass(1));
	blocks_[1].reset(new Block(2));
	blocks_[2].reset(new Block(3));
}

Manager::img_t Manager::loadTexture(const std::string& fn)
{
	using lib::img::simple::image;
	using lib::img::simple::read_simple_bmp;

	image img(read_simple_bmp(fn));
	DWORD id(gl::create_texture(raw(img), img.width, img.height, gl::RGBA, gl::NEAR));
	img_t r{id, img.width, img.height};

	impl_->imgs_.push_back(r);

	return r;
}

const Block& Manager::getBlock(int id) const
{
	assert(id>0&&id<=MXT_BLOCKCOUNT);
	return *impl_->blocks_[id-1];
}

Manager::Manager(void) : impl_(new Impl)
{
}

Manager::~Manager(void)
{
	delete impl_;
}

