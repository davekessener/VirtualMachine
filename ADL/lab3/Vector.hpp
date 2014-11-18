#ifndef DAV_VECTOR_H
#define DAV_VECTOR_H

#define MXT_VEC_STD_SIZE 16

namespace dav
{
	namespace vector_aux
	{
		template<typename T, typename A>
		class impl
		{
			public:
			typedef typename std::decay<T>::type value_type;
			typedef value_type *pointer;
			typedef const value_type *const_pointer;
			typedef value_type& reference;
			typedef const value_type& const_reference;
			typedef size_t size_type;
			typedef ptrdiff_t difference_type;
			typedef impl<T, A> self;
			
			public:
				impl(size_type c) : alloc_(), v_(c ? alloc_.allocate(c) : nullptr), c_(c), i_(0)
					{ assert(v_); }
				~impl( ) noexcept { destroy(v_, v_ + i_); alloc_.deallocate(v_, c_); }
				template<typename I> void create(I i, const_reference v) { new(&*i) value_type(v); }
				template<typename I, typename II> void create(I i1, I i2, II vi) { while(i1 != i2) { create(i1, *vi); ++i1; ++vi; } }
				template<typename I> void destroy(I i) noexcept { i->~value_type(); }
				template<typename I> void destroy(I i1, I i2) noexcept { while(i1 != i2) { destroy(i1); ++i1; } }
				void swap(self& v) noexcept { std::swap(v_, v.v_); std::swap(c_, v.c_); std::swap(i_, v.i_); }
			public:
				A alloc_;
				pointer v_;
				size_type c_, i_;
		};
	}

	template<typename T, typename Alloc = std::allocator<T>, typename MT = dav::SingleThreaded>
	class Vector : protected MT
	{
#define MXT_VECB(i) assert(i>=impl_.v_&&i<=impl_.v_+impl_.i_)
#define MXT_VECBC(i) assert(i>=impl_.v_&&i<impl_.v_+impl_.i_)
		typedef vector_aux::impl<T, Alloc> impl;
		typedef typename impl::value_type value_type;
		typedef typename impl::pointer pointer;
		typedef typename impl::const_pointer const_pointer;
		typedef typename impl::reference reference;
		typedef typename impl::const_reference const_reference;
		typedef typename impl::size_type size_type;
		typedef typename impl::difference_type difference_type;
		typedef pointer iterator;
		typedef const_pointer const_iterator;
		typedef std::reverse_iterator<iterator> reverse_iterator;
		typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
		typedef Vector<T, Alloc, MT> self;

		public:
			explicit Vector(size_type s = MXT_VEC_STD_SIZE) : Vector(s, nullptr, nullptr) { }
			template<typename I> vector(I i1, I i2) : Vector(std::distance(i1, i2), i1, i2) { }
			explicit vector(const std::initializer_list<value_type>& il) : Vector(il.size(), il.cbegin(), il.cend()) { }
			inline void push_back(const_reference v) { push(v); }
			inline void push_front(const_reference v) { push(v); }
			void push(const_reference);
			void insert(iterator i, const_reference v)
				{ MXT_VECB(i); if(i == impl_.v_ + impl_.i_) push(v); else i_insert(i, &v, &v + 1, 1); }
			template<typename I> void insert(iterator i, I i1, I i2);
				{ MXT_VECB(i); if(i == impl_.v_ + impl_.i_) while(i1 != i2) { push(*i1); ++i1; } else i_insert(i, i1, i2, std::distance(i1, i2)); }
			void pop_back( );
			void pop_front( ) { assert(impl_.i_); erase(impl_.v_, impl_.v_ + 1); }
			void erase(interator i) { MXT_VECBC(i); erase(i, i + 1); }
			void erase(iterator i1, iterator i2) { MXT_VECBC(i1); MXT_VECB(i2); assert(i1<i2); i_erase(i1, i2); }
			reference front( ) { assert(impl_.i_); return *impl_.v_; }
			reference back( ) { assert(impl_.i_); return *(impl_.v_ + impl_.i_); }
			const_reference front( ) const { assert(impl_.i_); return *impl_.v_; }
			const_reference back( ) const { assert(impl_.i_); return *(impl_.v_ + impl_.i_); }
			reference at(difference_type p) { assert(p<impl_.i_); return *(impl_.v_ + p); }
			reference operator[](difference_type p) { return *(impl_.v_ + p); }
			const_reference at(difference_type p) const { assert(p<impl_.i_); return *(impl_.v_ + p); }
			const_reference operator[](difference_type p) const { return *(impl_.v_ + p); }
			iterator begin( ) { return impl_.v_; }
			iterator end( ) { return impl_.v_ + impl_.i_; }
			const_iterator begin( ) const { return impl_.v_; }
			const_iterator end( ) const { return impl_.v_ + impl_.i_; }
			const_iterator cbegin( ) const { return impl_.v_; }
			const_iterator cend( ) const { return impl_.v_ + impl_.i_; }
			reverse_iterator rbegin( ) { return reverse_iterator(impl_.v_ + impl_.i_); }
			reverse_iterator rend( ) { return reverse_iterator(impl_.v_); }
			const_reverse_iterator rbegin( ) const { return const_reverse_iterator(impl_.v_ + impl_.i_); }
			const_reverse_iterator rend( ) const { return const_reverse_iterator(impl_.v_); }
		private:
			template<typename I> Vector(size_type, I, I);
			template<typename I> void i_insert(iterator, I, I, size_type);
			void i_erase(iterator, iterator);
		private:
			impl impl_;
	};

// # ===========================================================================

	VVV::VVV(size_type s, I i1, I i2) : impl_(s)
	{
		if(i1 != nullptr) impl_.create(impl_.v_, impl_.v_ + impl_.i_, i1);
	}

	void VVV::push(reference_type v)
	{
		if(impl_.i_ == impl_.c_)
		{
			impl tmp(2 * impl_.c_ + 1);
			tmp.create(tmp.v_, tmp.v_ + impl_.i_, impl_.v_);
			tmp.i_ = impl_.i_;
			impl_.swap(tmp);
		}

		impl_.create(impl_.v_ + impl_.i_, v);
		++impl_.i_;
	}

	template<typename I>
	void VVV::i_insert(iterator i, I i1, I i2, size_type d)
	{
		if(impl_.i_ + d > impl_.c_)
		{
			impl tmp(std::max(2 * impl_.c_ + 1, impl_.c_ + d));
			tmp.i_ = i - impl_.v_;
			if(tmp.i_) tmp.create(tmp.v_, tmp.v_ + tmp.i_, impl_.v_);
			tmp.create(tmp.v_ + tmp.i_, tmp.v_ + tmp.i_ + d, i1);
			tmp.i_ += d;
			tmp.create(tmp.v_ + tmp.i_, tmp.v_ + tmp.i_ + ((impl_.v_ + impl_.i_) - i), i);
			tmp.i_ += (impl_.v + impl_.i_) - i;
			impl_.swap(tmp);
		}
		else
		{
			impl_.create(rbegin(), reverse_iterator(i);
		}
	}
}

#undef MXT_VEC_STD_SIZE

#endif

