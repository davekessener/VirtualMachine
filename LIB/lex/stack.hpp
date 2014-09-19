#ifndef DAV_STACK_H
#define DAV_STACK_H

namespace dav
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

	template<typename T, typename Alloc>
	class StackImpl
	{
		public:
			StackImpl(int);
			~StackImpl( );
			template<typename TT> void swap(TT&, TT&) noexcept;
			void swap(StackImpl<T, Alloc>&) noexcept;
			template<typename I> void construct(I, const T&);
			template<typename I> void destroy(I);
			template<typename I> void destroy(I, I);
		public:
			T * v_;
			size_t i_, c_;
			Alloc alloc_;
		private:
			StackImpl(const StackImpl<T, Alloc>&) = delete;
			StackImpl<T, Alloc>& operator=(const StackImpl&) = delete;
	};

// # ===========================================================================

	template<typename T, typename MT = SingleThreaded, typename E = ErrorThrow<>, typename Alloc = std::allocator<T>>
	class Stack : protected MT
	{
		public:
		typedef T value_type;
 		typedef typename MT::mutex_type mutex_type;
		typedef typename E::error_type error_type;

		public:
			Stack(int = 0);
			Stack(const Stack<T, MT, E, Alloc>&);
			~Stack( );
			Stack<T, MT, E, Alloc>& operator=(const Stack&);
			size_t size( ) const;
			bool empty( ) const { return !size(); }
			void push(const T&);
			T& top( );
			void pop( );
		private:
			StackImpl<T, Alloc> i_;
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
	size_t Stack<T, MT, E, A>::size(void) const
	{
		return i_.i_;
	}

	template<typename T, typename MT, typename E, typename A>
	void Stack<T, MT, E, A>::push(const T& e)
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
	T& Stack<T, MT, E, A>::top(void)
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

	template<typename T, typename Alloc>
	StackImpl<T, Alloc>::StackImpl(int c)
//		: v_(static_cast<T *>(c > 0 ? operator new(c * sizeof(T)) : NULL)),
		: v_(alloc_.allocate(c)),
		  i_(0), c_(c)
	{
	}

	template<typename T, typename Alloc>
	StackImpl<T, Alloc>::~StackImpl(void)
	{
		destroy(v_, v_ + i_);
		alloc_.deallocate(v_, c_);
//		operator delete(v_);
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
	void StackImpl<T, Alloc>::construct(I p, const T& t)
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

#endif

