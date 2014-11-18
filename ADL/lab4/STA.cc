#include "STA.h"

namespace dav
{
	namespace sta
	{
		void *Block::allocate( )
		{
			byte *r = next_;
			next_ = data_ + *r * n_;

			--c_;

			return r;
		}

		void Block::deallocate(void *vp)
		{
			byte *p = static_cast<byte *>(vp);

			if(!c_)
			{
				next_ = p;
				*next_ = 0;
			}
			else if(p < next_)
			{
				*p = (next_ - data_) / n_;
				next_ = p;
			}
			else
			{
				byte o = (p - data_) / n_;
				byte *t = next_;

				while(*t && *t < o) t = data_ + *t * n_;

				*p = *t;
				*t = o;
			}

			++c_;
		}

// # ===========================================================================

		void *Allocator::allocate(void)
		{
			if(curAlloc_ == blocks_.end())
			{
				blocks_.emplace_back(n_);
				curAlloc_ = std::prev(blocks_.end());
			}
	
			if(curAlloc_->full())
			{
				curAlloc_ = blocks_.begin();
				for(auto e(blocks_.end()) ; curAlloc_->full() && curAlloc_ != e ; ++curAlloc_);
	
				return allocate();
			}
	
			curDealloc_ = curAlloc_;
	
			return curAlloc_->allocate();
		}
	
		void Allocator::deallocate(void *p)
		{
			if(!p)
			{
				return;
			}
			else if(curDealloc_ != blocks_.end() && curDealloc_->contains(p))
			{
				curDealloc_->deallocate(p);
			}
			else
			{
				for(auto i(blocks_.begin()), e(blocks_.end()) ; i != e ; ++i)
				{
					if(i->contains(p))
					{
						i->deallocate(p);
						curDealloc_ = i;
	
						if(i->empty()) cleanup();
	
						return;
					}
				}
	
				throw std::string("ERR: invalid pointer!");
			}
		}
	
		void Allocator::cleanup(void)
		{
			bool fa = false, fd = false;
	
			for(auto i(blocks_.rbegin()), e(blocks_.rend()), d(blocks_.rend()) ; i != e ; ++i)
			{
				if(i->empty())
				{
					if(d != blocks_.rend())
					{
						block_iter base(std::next(d).base());
	
						if(base == curAlloc_) fa = true;
						if(base == curDealloc_) fd = true;
	
						blocks_.erase(base);
					}
	
					d = i;
				}
			}
	
			if(fa) curAlloc_ = blocks_.begin();
			if(fd) curDealloc_ = blocks_.begin();
		}
	}

// # ===========================================================================

	void *SmallTypeAllocator::allocate(size_t n)
	{
		auto i(allocs_.find(n));

		if(i == allocs_.end())
		{
			i = allocs_.emplace(n, alloc_t(n)).first;
		}

		return i->second.allocate();
	}

	void SmallTypeAllocator::deallocate(void *p, size_t n)
	{
		auto i(allocs_.find(n));

		if(i == allocs_.end()) throw std::string("ERR: invalid size");

		i->second.deallocate(p);
	}
}

