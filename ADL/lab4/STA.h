#ifndef DAV_STA_H
#define DAV_STA_H

#include <list>
#include <map>
#include <string>

namespace dav
{
	namespace sta
	{
		class Block
		{
			typedef unsigned char byte;
			static const unsigned MAX = static_cast<byte>(-1) + 1;

			public:
				Block(size_t n) : data_(new byte[MAX * n]), next_(data_), c_(MAX), n_(n)
					{ for(size_t i = 0 ; i < MAX ; ++i) data_[i * n_] = i + 1; data_[n_ * (MAX - 1)] = 0; }
				~Block( ) { delete[] data_; }
				void *allocate( );
				void deallocate(void *);
				bool full( ) const { return !c_; }
				bool empty( ) const { return c_ == MAX; }
				bool contains(void *p) const
					{ return    static_cast<byte *>(p) >= data_
							 && static_cast<byte *>(p) < data_ + MAX * n_; }
			private:
				byte *data_, *next_;
				unsigned c_, n_;
		};

// # ===========================================================================

		class Allocator
		{
			typedef sta::Block block_t;
			typedef std::list<block_t> block_list;
			typedef typename block_list::iterator block_iter;
	
			public:
				Allocator(size_t N) : n_(N) { blocks_.emplace_front(n_); curAlloc_ = curDealloc_ = blocks_.begin(); }
				void *allocate( );
				void deallocate(void *);
				void cleanup( );
			private:
				block_list blocks_;
				block_iter curAlloc_, curDealloc_;
				size_t n_;
		};
	}

// # ===========================================================================

	class SmallTypeAllocator
	{
		typedef sta::Allocator alloc_t;
		typedef std::map<size_t, alloc_t> alloc_map;

		public:
			static void *Allocate(size_t s) { return Instance().allocate(s); }
			static void Deallocate(void *p, size_t n) { return Instance().deallocate(p, n); }
		private:
			SmallTypeAllocator( ) = default;
			~SmallTypeAllocator( ) = default;
			SmallTypeAllocator(const SmallTypeAllocator&) = delete;
			SmallTypeAllocator& operator=(const SmallTypeAllocator&) = delete;
		private:
			static SmallTypeAllocator& Instance( ) { static SmallTypeAllocator sta; return sta; }
		private:
			void *allocate(size_t);
			void deallocate(void *, size_t);
		private:
			alloc_map allocs_;
	};

// # ===========================================================================

	class SmallType
	{
		public:
			static void *operator new(size_t n) { return SmallTypeAllocator::Allocate(n); }
			static void operator delete(void *p, size_t n) { return SmallTypeAllocator::Deallocate(p, n); }
	};
}

#endif

