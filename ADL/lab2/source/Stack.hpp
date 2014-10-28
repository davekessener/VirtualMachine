#ifndef DAV_STACK_H
#define DAV_STACK_H

#include <string>
#include <cstddef>
#include <cassert>

/*
 * Based on H. Sutter's exception-safe stack implementation
 * ("Exceptional C++", 00), adding basic multithreading
 * support based on A. Alexandrescu's multithreading
 * facilities in "Modern C++ Design", 01
 */

namespace dav
{
	namespace stack
	{
		namespace aux
		{
			class SingleThreaded
			{
				public:
				typedef void mutex_type;

				public:
					class Lock
					{
					};
			};

			template<typename MTX>
			class MultiThreadedObject
			{
				public:
				typedef MTX mutex_type;

				public:
					class Lock
					{
						public:
							Lock( ) { mtx_.lock(); }
							~Lock( ) { mtx_.release(); }
					};
				private:
					MTX mtx_;
			};

			template<typename MTX>
			class MultiThreadedClass
			{
				public:
				typedef MTX mutex_type;

				public:
					class Lock
					{
						public:
							Lock( ) { mtx_.lock(); }
							~Lock( ) { mtx_.release(); }
					};
				private:
					static MTX mtx_;
			};
			template<typename MTX>
			MTX MultiThreadedClass<MTX>::mtx_;

			template<typename E = std::string>
			class ErrorThrow
			{
				public:
				typedef E error_type;

				public:
					static void OnError(const E& e) { throw e; }
			};

			template<typename E = std::string>
			class ErrorSilent
			{
				public:
				typedef void error_type;

				public:
					static void OnError(const E& e) { }
			};

// # ===========================================================================

			template<typename T, typename TT>
			struct transfer_const
			{
				typedef TT type;
			};

			template<typename T, typename TT>
			struct transfer_const<const T, TT>
			{
				typedef const TT type;
			};

			template<typename T>
			class stack_iterator
			{
				typedef T stack_impl;

				public:
				typedef std::random_access_iterator_tag iterator_category;
				typedef typename stack_impl::value_type value_type;
				typedef typename transfer_const<stack_impl, typename stack_impl::pointer>::type pointer;
				typedef typename transfer_const<stack_impl, typename stack_impl::reference>::type reference;
				typedef typename stack_impl::const_pointer const_pointer;
				typedef typename stack_impl::const_reference const_reference;
				typedef typename stack_impl::difference_type difference_type;
				typedef typename stack_impl::size_type size_type;

				public:
					stack_iterator( ) : c_(nullptr), pos_(0) { }
					stack_iterator(stack_impl &s, size_type pos) : c_(&s), pos_(pos) { }
					stack_iterator(const stack_iterator<typename std::decay<stack_impl>::type>& i) : c_(i.c_), pos_(i.pos_) { }
					reference operator[](difference_type i) { assert(pos_+i>=0&&pos_+i<(difference_type)c_->i_); return c_->v_[pos_ + i]; }
					reference operator*( ) { return (*this)[0]; }
					pointer operator->( ) { return &(*this)[0]; }
					bool operator==(const stack_iterator<T>& it) const { return c_ == it.c_ && pos_ == it.pos_; }
					bool operator<(const stack_iterator<T>& it) const { return c_ == it.c_ && pos_ < it.pos_; }
					stack_iterator<T>& operator+=(difference_type o) { pos_ += o; return *this; }
					stack_iterator<T>& operator-=(difference_type o) { pos_ -= o; return *this; }
					difference_type operator-(const stack_iterator<T>& i) const { assert(c_==i.c_); return pos_ - i.pos_; }
					stack_iterator<T>& operator++( ) { ++pos_; return *this; }
					stack_iterator<T>  operator++(int) { stack_iterator<T> t(*this); ++*this; return t; }
					stack_iterator<T>& operator--( ) { --pos_; return *this; }
					stack_iterator<T>  operator--(int) { stack_iterator<T> t(*this); --*this; return t; }
					void swap(stack_iterator<T>& i) noexcept { std::swap(c_, i.c_); std::swap(pos_, i.pos_); }
				private:
					stack_impl *c_;
					difference_type pos_;
				private:
					friend class stack_iterator<const typename std::decay<stack_impl>::type>;
			};

			template<typename T> bool operator!=(const stack_iterator<T>& i1, const stack_iterator<T>& i2) { return !(i1 == i2); }
			template<typename T> bool operator<=(const stack_iterator<T>& i1, const stack_iterator<T>& i2) { return i1 == i2 || i1 < i2; }
			template<typename T> bool operator>(const stack_iterator<T>& i1, const stack_iterator<T>& i2) { return !(i1 == i2 || i1 < i2); }
			template<typename T> bool operator>=(const stack_iterator<T>& i1, const stack_iterator<T>& i2) { return !(i1 < i2); }
			template<typename T> stack_iterator<T> operator+(const stack_iterator<T>& i, typename stack_iterator<T>::difference_type o) { stack_iterator<T> t(i); t += o; return t; }
			template<typename T> stack_iterator<T> operator-(const stack_iterator<T>& i, typename stack_iterator<T>::difference_type o) { stack_iterator<T> t(i); t -= o; return t; }
			template<typename T> stack_iterator<T> operator+(typename stack_iterator<T>::difference_type o, const stack_iterator<T>& i) { stack_iterator<T> t(i); t += o; return t; }
			template<typename T> void swap(stack_iterator<T>& i1, stack_iterator<T>& i2) noexcept { i1.swap(i2); }

// # ===========================================================================

			template<typename T, typename Alloc>
			class StackImpl
			{
				public:
				typedef T value_type;
				typedef value_type *pointer;
				typedef const value_type *const_pointer;
				typedef value_type &reference;
				typedef const value_type &const_reference;
				typedef ptrdiff_t difference_type;
				typedef size_t size_type;

				public:
					StackImpl(int);
					~StackImpl( );
					template<typename TT> void swap(TT&, TT&) noexcept;
					void swap(StackImpl<T, Alloc>&) noexcept;
					template<typename I> void construct(I, const_reference);
					template<typename I> void destroy(I);
					template<typename I> void destroy(I, I);
				public:
					pointer v_;
					size_type i_, c_;
					Alloc alloc_;
				private:
					StackImpl(const StackImpl<T, Alloc>&) = delete;
					StackImpl<T, Alloc>& operator=(const StackImpl&) = delete;
			};
		}

// # ===========================================================================

		template
		<
			typename T,
			typename MT = aux::SingleThreaded,
			typename E = aux::ErrorThrow<>,
			typename Alloc = std::allocator<typename std::decay<T>::type>
		>
		class Stack : protected MT
		{
			public:
			typedef aux::StackImpl<typename std::decay<T>::type, Alloc> stack_impl;
			typedef typename stack_impl::value_type value_type;
			typedef typename stack_impl::pointer pointer;
			typedef typename stack_impl::const_pointer const_pointer;
			typedef typename stack_impl::reference reference;
			typedef typename stack_impl::const_reference const_reference;
			typedef typename stack_impl::difference_type difference_type;
			typedef typename stack_impl::size_type size_type;
			typedef aux::stack_iterator<stack_impl> iterator;
			typedef aux::stack_iterator<const stack_impl> const_iterator;
			typedef std::reverse_iterator<iterator> reverse_iterator;
			typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
 			typedef typename MT::mutex_type mutex_type;
			typedef typename E::error_type error_type;

			public:
				Stack(int = 0);
				Stack(const Stack<T, MT, E, Alloc>&);
				~Stack( );
				Stack<T, MT, E, Alloc>& operator=(const Stack&);
				size_type size( ) const;
				bool empty( ) const { return !size(); }
				void push(const_reference);
				reference top( );
				const_reference top( ) const;
				void pop( );
				iterator begin( ) { return iterator(i_, 0); }
				iterator end( ) { return iterator(i_, i_.i_); }
				const_iterator begin( ) const { return const_iterator(i_, 0); }
				const_iterator end( ) const { return const_iterator(i_, i_.i_); }
				const_iterator cbegin( ) const { return const_iterator(i_, 0); }
				const_iterator cend( ) const { return const_iterator(i_, i_.i_); }
				reverse_iterator rbegin( ) { return reverse_iterator(end()); }
				reverse_iterator rend( ) { return reverse_iterator(begin()); }
				const_reverse_iterator rbegin( ) const { return const_reverse_iterator(cend()); }
				const_reverse_iterator rend( ) const { return const_reverse_iterator(cbegin()); }
				const_reverse_iterator crbegin( ) const { return const_reverse_iterator(cend()); }
				const_reverse_iterator crend( ) const { return const_reverse_iterator(cbegin()); }
			private:
				stack_impl i_;
		};

// # ---------------------------------------------------------------------------

#define MXT_LOCK typename MT::Lock()

		template<typename T, typename MT, typename E, typename A>
		Stack<T, MT, E, A>::Stack(int c) : i_(c)
		{
		}

		template<typename T, typename MT, typename E, typename A>
		Stack<T, MT, E, A>::Stack(const Stack<T, MT, E, A>& s) : i_(s.i_)
		{
			while(i_.i_ < i_.c_)
			{
				push(s.v_[i_.i_]);
				++i_.i_;
			}
		}

		template<typename T, typename MT, typename E, typename A>
		Stack<T, MT, E, A>::~Stack(void)
		{
		}

		template<typename T, typename MT, typename E, typename A>
		Stack<T, MT, E, A>& Stack<T, MT, E, A>::operator=(const Stack<T, MT, E, A>& o)
		{
			Stack<T, MT, E, A> t(o);
			
			i_.swap(t.i_);

			return *this;
		}

		template<typename T, typename MT, typename E, typename A>
		typename Stack<T, MT, E, A>::size_type Stack<T, MT, E, A>::size(void) const
		{
			return i_.i_;
		}

		template<typename T, typename MT, typename E, typename A>
		void Stack<T, MT, E, A>::push(const_reference e)
		{
			MXT_LOCK;

			if(i_.i_ == i_.c_)
			{
				Stack<T, MT, E, A> t(i_.c_ * 2 + 1);

				while(t.i_.i_ < i_.i_)
				{
					t.push(i_.v_[t.i_.i_]);
				}

				t.push(e);

				i_.swap(t.i_);
			}
			else
			{
				i_.construct(i_.v_ + i_.i_, e);
				++i_.i_;
			}
		}

		template<typename T, typename MT, typename E, typename A>
		typename Stack<T, MT, E, A>::reference Stack<T, MT, E, A>::top(void)
		{
			if(i_.i_ == 0)
			{
				E::OnError("empty stack");
			}

			return i_.v_[i_.i_ - 1];
		}

		template<typename T, typename MT, typename E, typename A>
		typename Stack<T, MT, E, A>::const_reference Stack<T, MT, E, A>::top(void) const
		{
			if(i_.i_ == 0)
			{
				E::OnError("empty stack");
			}

			return i_.v_[i_.i_ - 1];
		}

		template<typename T, typename MT, typename E, typename A>
		void Stack<T, MT, E, A>::pop(void)
		{
			MXT_LOCK;

			if(i_.i_ == 0)
			{
				E::OnError("cannot pop from empty stack");
			}

			--i_.i_;
			i_.destroy(i_.v_ + i_.i_);

			if(i_.i_ < i_.c_ / 4)
			{
				Stack<T, MT, E, A> t(i_.c_ / 2);

				while(t.i_.i_ < i_.i_)
				{
					t.push(i_.v_[t.i_.i_]);
				}

				i_.swap(t.i_);
			}
		}

#undef MXT_LOCK

// # ===========================================================================

		namespace aux
		{
			template<typename T, typename Alloc>
			StackImpl<T, Alloc>::StackImpl(int c)
				: v_(alloc_.allocate(c)),
				  i_(0), c_(c)
			{
			}

			template<typename T, typename Alloc>
			StackImpl<T, Alloc>::~StackImpl(void)
			{
				destroy(v_, v_ + i_);
				alloc_.deallocate(v_, c_);
			}

			template<typename T, typename Alloc>
			template<typename TT>
			void StackImpl<T, Alloc>::swap(TT& t1, TT& t2) noexcept
			{
				TT t(t1);
				t1 = t2;
				t2 = t;
			}

			template<typename T, typename Alloc>
			void StackImpl<T, Alloc>::swap(StackImpl<T, Alloc>& o) noexcept
			{
				swap(v_, o.v_);
				swap(i_, o.i_);
				swap(c_, o.c_);
			}

			template<typename T, typename Alloc>
			template<typename I>
			void StackImpl<T, Alloc>::construct(I p, const_reference t)
			{
				new(&*p) T(t);
			}

			template<typename T, typename Alloc>
			template<typename I>
			void StackImpl<T, Alloc>::destroy(I p)
			{
				p->~T();
			}

			template<typename T, typename Alloc>
			template<typename I>
			void StackImpl<T, Alloc>::destroy(I p1, I p2)
			{
				while(p1 != p2)
				{
					destroy(p1);
					++p1;
				}
			}
		}
	}
}

#endif

