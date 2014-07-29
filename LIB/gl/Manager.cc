#include <array>
#include <memory>
#include <cassert>
#include "Manager.h"
#include "Block.h"
#include "BlockGrass.h"

#define MXT_BLOCKCOUNT 3

Manager& Manager::instance(void)
{
	static Manager m;
	return m;
}

struct Manager::Impl
{
	Impl( );
	std::array<std::shared_ptr<Block>, MXT_BLOCKCOUNT> blocks_;
};

Manager::Impl::Impl(void)
{
	blocks_[0].reset(new BlockGrass(1));
	blocks_[1].reset(new Block(2));
	blocks_[2].reset(new Block(3));
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

