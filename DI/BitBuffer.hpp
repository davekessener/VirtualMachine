#ifndef DAV_BITBUFFER_H
#define DAV_BITBUFFER_H

namespace dav
{
	namespace bitbuffer
	{
		typedef unsigned long long largest_t;
		typedef unsigned int uint;
		const uint largest_size(sizeof(largest_t) * 8);

#ifndef MXT_STATIC_BITBUFFER
		class BitBufferUnit
		{
			public:
				BitBufferUnit(uint s) : max_(s), v_(0), mask_(static_cast<largest_t>(-1) >> (largest_size - s)) { }
				bool empty( ) const { return !v_; }
				bool get(uint i) const { assert(i<max_); return v_ & (1 << i); }
				bool inc( ) { largest_t t((v_ + 1) & mask_); bool f(t < v_); v_ = t; return f; }
			private:
				uint max_;
				largest_t v_, mask_;
		};
	
		class BitBufferImpl
		{
			public:
				BitBufferImpl(uint s) : max_(s), v_(s < largest_size ? s : largest_size)
					{ if(max_ > largest_size) next_.reset(new BitBufferImpl(max_ - largest_size)); }
				bool empty( ) const { return v_.empty() && (next_ ? next_->empty() : true); }
				bool get(uint i) const { assert(i<max_); return i >= largest_size ? next_->get(i - largest_size) : v_.get(i); }
				void inc( ) { if(v_.inc() && next_) next_->inc(); }
			private:
				uint max_;
				BitBufferUnit v_;
				std::shared_ptr<BitBufferImpl> next_;
		};

		class BitBuffer
		{
			public:
				BitBuffer(uint s) : s_(s), v_(s) { }
				bool empty( ) const { return v_.empty(); }
				bool get(uint i) const { assert(i<s_); return v_.get(s_ - i - 1); }
				void increment( ) { v_.inc(); }
			private:
				uint s_;
				BitBufferImpl v_;
		};
#else
		template<uint V>
		struct Exceeds
		{
			enum { value = V > largest_size };
		};
		
		template<uint V, uint F>
		struct BitBufferImpl;

		template<uint V>
		struct BitBufferImpl<V, 0>
		{
			static_assert(V&&V<=largest_size, "Uhm...");
			static const largest_t mask = static_cast<largest_t>(-1) >> (largest_size - V);

			public:
				BitBufferImpl( ) : v_(0) { }
				bool empty( ) const { return !v_; }
				bool get(uint idx) const { assert(idx < V); return v_ & (1 << idx); }
				bool increment( ) { largest_t t((v_ + 1) & mask); bool f(t < v_); v_ = t; return f; }
			private:
				largest_t v_;
		};

		template<uint V, uint F>
		struct BitBufferImpl
		{
			public:
				bool empty( ) const { return v_.empty() && self_.empty(); }
				bool get(uint idx) const { return idx >= largest_size ? self_.get(idx - largest_size) : v_.get(idx); }
				void increment( ) { if(v_.increment()) self_.increment(); }
			private:
				static const uint next = V - largest_size;
				typedef BitBufferImpl<next, Exceeds<next>::value> super;
				typedef BitBufferImpl<largest_size, 0> unit;

				super self_;
				unit v_;
		};

		template<uint V>
		class BitBuffer
		{
			typedef bitbuffer::BitBufferImpl<V, bitbuffer::Exceeds<V>::value> super;
			public:
				bool empty( ) const { return self_.empty(); }
				bool get(bitbuffer::uint idx) const { assert(idx < V); return self_.get(V - idx - 1); }
				void increment( ) { self_.increment(); }
			private:
				super self_;
		};
#endif
	}
}

#endif

