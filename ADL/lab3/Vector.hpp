#ifndef DAV_VECTOR_H
#ifndef DAV_VECTOR_H

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
				template<typename I> void create(I i1, I i2, I vi) { while(i1 != i2) { create(i1, *vi); ++i1; ++vi; } }
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
		typedef vector_aux::impl impl;

		public:
			explicit Vector(size_type = MXT_VEC_STD_SIZE);
			template<typename I> vector(I, I);
			explicit vector(const std::initializer_list<value_type>&);
		private:
			impl impl_;
	};
}

#undef MXT_VEC_STD_SIZE

#endif

