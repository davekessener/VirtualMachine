#ifndef LIB_DAV_STABLEVECTOR_H
#define LIB_DAV_STABLEVECTOR_H

#include <utility>
#include <map>
#include <cassert>

#define MXT_DEFAULTCAP 16

namespace dav
{
	template<typename T>
	struct stable_vector;

	template<typename T>
	class stable_vector_iterator
	{
		public:
		typedef stable_vector<T> vec_t;
		typedef typename vec_t::value_type value_type;
		typedef typename vec_t::ptr_t ptr_t;
		typedef typename vec_t::id_t id_t;

		public:
			stable_vector_iterator(const vec_t * = NULL, id_t = 0);
			stable_vector_iterator(const stable_vector_iterator&);
			stable_vector_iterator(stable_vector_iterator&&);
			stable_vector_iterator& operator=(const stable_vector_iterator&);
			stable_vector_iterator& operator=(stable_vector_iterator&&);
			void swap(stable_vector_iterator&) noexcept;
			value_type& operator*( );
			const value_type& operator*( ) const;
			value_type *operator->( );
			const value_type *operator->( ) const;
			stable_vector_iterator& operator++( );
			const stable_vector_iterator& operator++( ) const;
			stable_vector_iterator operator++(int);
			const stable_vector_iterator operator++(int) const;
			stable_vector_iterator& operator--( );
			const stable_vector_iterator& operator--( ) const;
			stable_vector_iterator operator--(int);
			const stable_vector_iterator operator--(int) const;
			bool operator==(const stable_vector_iterator&) const;
			bool operator!=(const stable_vector_iterator&) const;
			bool operator<=(const stable_vector_iterator&) const;
			bool operator>=(const stable_vector_iterator&) const;
			bool operator<(const stable_vector_iterator&) const;
			bool operator>(const stable_vector_iterator&) const;
			size_t operator-(const stable_vector_iterator&) const;
			stable_vector_iterator operator+(int);
			const stable_vector_iterator operator+(int) const;
			stable_vector_iterator operator-(int);
			const stable_vector_iterator operator-(int) const;
			value_type& operator[](int);
			const value_type& operator[](int) const;
		private:
			void refresh( ) const;
			void reassign(ptr_t) const;
		private:
			const vec_t *vec_;
			mutable id_t id_;
			mutable ptr_t ptr_;
	};

// # ===========================================================================

	template<typename TT>
	class stable_vector
	{
		public:
		typedef TT value_type;
		typedef value_type T;
		typedef T *ptr_t;
		typedef const T *cptr_t;
		typedef size_t id_t;
		typedef stable_vector_iterator<T> iterator;
		typedef const stable_vector_iterator<T> const_iterator;

		public:
			explicit stable_vector(size_t = 0, size_t = MXT_DEFAULTCAP);
			stable_vector(const T *, size_t);
			template<typename I>
				stable_vector(I, I);
			stable_vector(const stable_vector&);
			stable_vector(stable_vector&&);
			~stable_vector( ) noexcept;
			void swap(stable_vector&) noexcept;
			size_t size( ) const;
			size_t capacity( ) const;
			bool empty( ) const;
			template<typename II, typename OI> void insert(II, II, OI);
			template<typename I> void insert(I, const T&);
			void push_back(const T&);
			void push_front(const T&);
			void push(const T&);
			void poke(const T&);
			template<typename I> void erase(I, I);
			template<typename I> void erase(I);
			T& front( );
			const T& front( ) const;
			T& operator[](int);
			const T& operator[](int) const;
			T& at(int);
			const T& at(int) const;
			iterator begin( );
			iterator end( );
			const_iterator begin( ) const;
			const_iterator end( ) const;
			const_iterator cbegin( ) const;
			const_iterator cend( ) const;
		private:
			template<typename I> void construct(I, const T&);
			template<typename I> void destroy(I, I);
			template<typename I> void destroy(I);
			void generate( );
		private:
			friend class stable_vector_iterator<T>;
			ptr_t getPtr(id_t) const;
			id_t getID(ptr_t) const;
		private:
			T *v_;
			size_t i_, c_, next_;
			std::map<id_t, ptr_t> lookup_;
			std::map<ptr_t, id_t> reverse_;
	};

// # ===========================================================================

	template<typename T>
	stable_vector<T>::stable_vector(size_t i, size_t c) : v_(NULL), i_(i), c_(i > c ? i : (c ? c : MXT_DEFAULTCAP)), next_(0)
	{
		v_ = static_cast<T *>(operator new(c_ * sizeof(T)));
		for(T *t = v_ ; i-- ;) construct(t++, T());
		generate();
	}

	template<typename T>
	stable_vector<T>::stable_vector(const T *v, size_t s) : stable_vector(v, v + s)
	{
	}

	template<typename T>
	template<typename I>
	stable_vector<T>::stable_vector(I i1, I i2) : v_(NULL), i_(0), c_(0), next_(0)
	{
		if(i1 != i2)
		{
			I i(i1);
			while(i != i2) { ++i; ++c_; }
			i_ = c_;
			v_ = static_cast<T *>(operator new(c_ * sizeof(T)));
			for(T *t = v_ ; i1 != i2 ; ++i1) construct(t++, *i1);
			generate();
		}
	}

	template<typename T>
	stable_vector<T>::stable_vector(const stable_vector<T>& v) : stable_vector(v.v_, v.v_ + v.i_)
	{
	}

	template<typename T>
	stable_vector<T>::stable_vector(stable_vector<T>&& v) : stable_vector(0)
	{
		swap(v);
	}

	template<typename T>
	stable_vector<T>::~stable_vector(void) noexcept
	{
		destroy(v_, v_ + i_);
		operator delete(v_);
	}

	template<typename T>
	void stable_vector<T>::swap(stable_vector<T>& v) noexcept
	{
		std::swap(v_, v.v_);
		std::swap(i_, v.i_);
		std::swap(c_, v.c_);
		std::swap(next_, v.next_);
		std::swap(lookup_, v.lookup_);
		std::swap(reverse_, v.reverse_);
	}

	template<typename T>
	size_t stable_vector<T>::size(void) const
	{
		return i_;
	}

	template<typename T>
	size_t stable_vector<T>::capacity(void) const
	{
		return c_;
	}

	template<typename T>
	bool stable_vector<T>::empty(void) const
	{
		return i_ == 0;
	}

	template<typename T>
	template<typename II, typename OI>
	void stable_vector<T>::insert(II i1, II i2, OI o)
	{
		while(i1 != i2)
		{
			insert(o, *i1);
			++o;
			++i1;
		}
	}

	template<typename T>
	template<typename I>
	void stable_vector<T>::insert(I i, const T& t)
	{
		if(i_ == c_)
		{
			stable_vector<T> v(0ul, 2 * c_ + 1);
			v.insert(v_, v_ + i_, v.v_);
			for(size_t j = 0 ; j < i_ ; ++j)
			{
				id_t id(reverse_[v_ + j]);
				v.lookup_[id] = v.v_ + j;
				v.reverse_[v.v_ + j] = id;
			}
			swap(v);
		}

		construct(v_ + i_, T());

		ptr_t const pt = &*i, e = v_ + i_;

		assert(pt>=v_&&pt<=e);
		
		for(ptr_t p = e ; p > pt ; --p)
		{
			p[0] = p[-1];
			reverse_[p] = reverse_[p - 1];
			lookup_[reverse_[p]] = p;
		}

		id_t id(++next_);
		lookup_[id] = pt;
		reverse_[pt] = id;
		*pt = t;

		++i_;
	}

	template<typename T>
	void stable_vector<T>::push_back(const T& t)
	{
		insert(end(), t);
	}

	template<typename T>
	void stable_vector<T>::push_front(const T& t)
	{
		insert(begin(), t);
	}

	template<typename T>
	void stable_vector<T>::push(const T& t)
	{
		push_back(t);
	}

	template<typename T>
	void stable_vector<T>::poke(const T& t)
	{
		push_front(t);
	}

	template<typename T>
	template<typename I>
	void stable_vector<T>::erase(I i1, I i2)
	{
		while(i1 != i2)
		{
			erase(i1);
			++i1;
		}
	}

	template<typename T>
	template<typename I>
	void stable_vector<T>::erase(I i)
	{
		--i_;

		ptr_t const pt = &*i, e = v_ + i_;
		
		assert(pt>=v_&&pt<=e);

		lookup_.erase(lookup_.find(reverse_[pt]));

		for(ptr_t p = pt ; p < e ; ++p)
		{
			p[0] = p[1];
			lookup_[reverse_[p + 1]] = p;
			reverse_[p] = reverse_[p + 1];
		}

		reverse_.erase(reverse_.find(e));
	}

	template<typename T>
	T& stable_vector<T>::front(void)
	{
		assert(v_&&i_);
		return *v_;
	}

	template<typename T>
	const T& stable_vector<T>::front(void) const
	{
		assert(v_&&i_);
		return *v_;
	}

	template<typename T>
	T& stable_vector<T>::operator[](int idx)
	{
		return v_[idx];
	}

	template<typename T>
	const T& stable_vector<T>::operator[](int idx) const
	{
		return v_[idx];
	}

	template<typename T>
	T& stable_vector<T>::at(int idx)
	{
		assert(v_&&idx>=0&&idx<i_);
		return v_[idx];
	}

	template<typename T>
	const T& stable_vector<T>::at(int idx) const
	{
		assert(v_&&idx>=0&&idx<i_);
		return v_[idx];
	}

	template<typename T>
	typename stable_vector<T>::iterator stable_vector<T>::begin(void)
	{
		return iterator(this, i_ ? reverse_[v_] : 0);
	}

	template<typename T>
	typename stable_vector<T>::iterator stable_vector<T>::end(void)
	{
		return iterator(this, 0);
	}

	template<typename T>
	typename stable_vector<T>::const_iterator stable_vector<T>::begin(void) const
	{
		return const_iterator(this, i_ ? reverse_.at(v_) : 0);
	}

	template<typename T>
	typename stable_vector<T>::const_iterator stable_vector<T>::end(void) const
	{
		return const_iterator(this, 0);
	}

	template<typename T>
	typename stable_vector<T>::const_iterator stable_vector<T>::cbegin(void) const
	{
		return const_iterator(this, i_ ? reverse_.at(v_) : 0);
	}

	template<typename T>
	typename stable_vector<T>::const_iterator stable_vector<T>::cend(void) const
	{
		return const_iterator(this, 0);
	}

// # ---------------------------------------------------------------------------

	template<typename T>
	template<typename I>
	void stable_vector<T>::construct(I i, const T& t)
	{
		new (&*i) T(t);
	}

	template<typename T>
	template<typename I>
	void stable_vector<T>::destroy(I i1, I i2)
	{
		while(i1 != i2)
		{
			destroy(i1);
			++i1;
		}
	}

	template<typename T>
	template<typename I>
	void stable_vector<T>::destroy(I i)
	{
		(&*i)->~T();
	}

	template<typename T>
	void stable_vector<T>::generate(void)
	{
		for(ptr_t p = v_, e = v_ + i_ ; p < e ; ++p)
		{
			id_t id(++next_);
			lookup_[id] = p;
			reverse_[p] = id;
		}
	}

// # ---------------------------------------------------------------------------

	template<typename T>
	typename stable_vector<T>::ptr_t stable_vector<T>::getPtr(id_t id) const
	{
		return id ? lookup_.at(id) : v_ + i_;
	}

	template<typename T>
	typename stable_vector<T>::id_t stable_vector<T>::getID(ptr_t p) const
	{
		return p == v_ + i_ ? 0 : reverse_.at(p);
	}

// # ===========================================================================

	template<typename T>
	stable_vector_iterator<T>::stable_vector_iterator(const vec_t *vec, id_t id) : vec_(vec), id_(id)
	{
	}

	template<typename T>
	stable_vector_iterator<T>::stable_vector_iterator(const stable_vector_iterator& i) : vec_(i.vec_), id_(i.id_)
	{
	}

	template<typename T>
	stable_vector_iterator<T>::stable_vector_iterator(stable_vector_iterator&& i)
	{
		swap(i);
	}

	template<typename T>
	stable_vector_iterator<T>& stable_vector_iterator<T>::operator=(const stable_vector_iterator<T>& i)
	{
		stable_vector_iterator<T> t(i);
		swap(t);
		return *this;
	}

	template<typename T>
	stable_vector_iterator<T>& stable_vector_iterator<T>::operator=(stable_vector_iterator<T>&& i)
	{
		swap(i);
		return *this;
	}

	template<typename T>
	void stable_vector_iterator<T>::swap(stable_vector_iterator<T>& i) noexcept
	{
		std::swap(vec_, i.vec_);
		std::swap(id_, i.id_);
	}

	template<typename T>
	typename stable_vector_iterator<T>::value_type& stable_vector_iterator<T>::operator*(void)
	{
		refresh();
		assert(ptr_);
		return *ptr_;
	}

	template<typename T>
	const typename stable_vector_iterator<T>::value_type& stable_vector_iterator<T>::operator*(void) const
	{
		refresh();
		assert(ptr_);
		return *ptr_;
	}

	template<typename T>
	typename stable_vector_iterator<T>::value_type *stable_vector_iterator<T>::operator->(void)
	{
		refresh();
		assert(ptr_);
		return ptr_;
	}

	template<typename T>
	const typename stable_vector_iterator<T>::value_type *stable_vector_iterator<T>::operator->(void) const
	{
		refresh();
		assert(ptr_);
		return ptr_;
	}

	template<typename T>
	stable_vector_iterator<T>& stable_vector_iterator<T>::operator++(void)
	{
		refresh();
		reassign(++ptr_);
		return *this;
	}

	template<typename T>
	const stable_vector_iterator<T>& stable_vector_iterator<T>::operator++(void) const
	{
		refresh();
		reassign(++ptr_);
		return *this;
	}

	template<typename T>
	stable_vector_iterator<T> stable_vector_iterator<T>::operator++(int)
	{
		stable_vector_iterator<T> t(*this);
		++*this;
		return t;
	}

	template<typename T>
	const stable_vector_iterator<T> stable_vector_iterator<T>::operator++(int) const
	{
		stable_vector_iterator<T> t(*this);
		++*this;
		return t;
	}

	template<typename T>
	stable_vector_iterator<T>& stable_vector_iterator<T>::operator--(void)
	{
		refresh();
		reassign(--ptr_);
		return *this;
	}

	template<typename T>
	const stable_vector_iterator<T>& stable_vector_iterator<T>::operator--(void) const
	{
		refresh();
		reassign(--ptr_);
		return *this;
	}

	template<typename T>
	stable_vector_iterator<T> stable_vector_iterator<T>::operator--(int)
	{
		stable_vector_iterator<T> t(*this);
		--*this;
		return t;
	}

	template<typename T>
	const stable_vector_iterator<T> stable_vector_iterator<T>::operator--(int) const
	{
		stable_vector_iterator<T> t(*this);
		--*this;
		return t;
	}

	template<typename T>
	bool stable_vector_iterator<T>::operator==(const stable_vector_iterator<T>& i) const
	{
		refresh();
		i.refresh();
		return ptr_ == i.ptr_;
	}

	template<typename T>
	bool stable_vector_iterator<T>::operator!=(const stable_vector_iterator<T>& i) const
	{
		return !operator==(i);
	}

	template<typename T>
	bool stable_vector_iterator<T>::operator<=(const stable_vector_iterator<T>& i) const
	{
		refresh();
		i.refresh();
		return ptr_ <= i.ptr_;
	}

	template<typename T>
	bool stable_vector_iterator<T>::operator>=(const stable_vector_iterator<T>& i) const
	{
		refresh();
		i.refresh();
		return ptr_ >= i.ptr_;
	}

	template<typename T>
	bool stable_vector_iterator<T>::operator<(const stable_vector_iterator<T>& i) const
	{
		return !operator>=(i);
	}

	template<typename T>
	bool stable_vector_iterator<T>::operator>(const stable_vector_iterator<T>& i) const
	{
		return !operator<=(i);
	}

	template<typename T>
	size_t stable_vector_iterator<T>::operator-(const stable_vector_iterator<T>& i) const
	{
		return *this <= i ? i.ptr_ - ptr_ : ptr_ - i.ptr_;
	}

	template<typename T>
	stable_vector_iterator<T> stable_vector_iterator<T>::operator+(int d)
	{
		stable_vector_iterator<T> i(*this);
		refresh();
		i.reassign(ptr_ + d);
		return i;
	}

	template<typename T>
	const stable_vector_iterator<T> stable_vector_iterator<T>::operator+(int d) const
	{
		stable_vector_iterator<T> i(*this);
		refresh();
		i.reassign(ptr_ + d);
		return i;
	}

	template<typename T>
	stable_vector_iterator<T> stable_vector_iterator<T>::operator-(int d)
	{
		return operator+(-d);
	}

	template<typename T>
	const stable_vector_iterator<T> stable_vector_iterator<T>::operator-(int d) const
	{
		return operator+(-d);
	}

	template<typename T>
	typename stable_vector_iterator<T>::value_type& stable_vector_iterator<T>::operator[](int d)
	{
		refresh();
		assert(ptr_);
		return ptr_[d];
	}

	template<typename T>
	const typename stable_vector_iterator<T>::value_type& stable_vector_iterator<T>::operator[](int d) const
	{
		refresh();
		assert(ptr_);
		return ptr_[d];
	}

// # ---------------------------------------------------------------------------

	template<typename T>
	void stable_vector_iterator<T>::refresh(void) const
	{
		ptr_ = vec_->getPtr(id_);
	}

	template<typename T>
	void stable_vector_iterator<T>::reassign(ptr_t ptr) const
	{
		ptr_ = ptr;
		id_ = vec_->getID(ptr_);
		assert(ptr_==vec_->getPtr(id_));
	}

// # ===========================================================================
}

namespace std
{
	template<typename T>
	struct iterator_traits<dav::stable_vector_iterator<T>>
	{
		typedef typename dav::stable_vector_iterator<T>::value_type value_type;
		typedef typename dav::stable_vector_iterator<T>::ptr_t pointer;
		typedef size_t difference_type;
		typedef value_type& reference;
		typedef random_access_iterator_tag iterator_category;
	};
}

#endif

