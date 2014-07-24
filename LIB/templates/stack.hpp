#ifndef SIU_STACK_H
#define SIU_STACK_H

namespace siu
{
	template<typename T>
	class _Stack_impl
	{
		public:
			_Stack_impl(int);
			~_Stack_impl( );
			template<typename TT> void swap(TT&, TT&) throw();
			void swap(_Stack_impl<T>&) throw();
			template<typename I> void construct(I, const T&);
			template<typename I> void destroy(I);
			template<typename I> void destroy(I, I);
		private:
			T *_v;
			size_t _i, _c;
		private:
			_Stack_impl(const _Stack_impl<T>&);
			_Stack_impl<T>& operator=(const _Stack_impl&);
	};

	template<typename T>
	class Stack
	{
		public:
			Stack(int = 0);
			Stack(const Stack<T>&);
			~Stack( );
			Stack<T>& operator=(const Stack&);
			size_t size( ) const;
			void push(const T&);
			T& top( );
			void pop( );
		private:
			_Stack_impl<T> _i;
	};

	template<typename T>
	Stack<T>::Stack(int c) : _i(c)
	{
	}

	template<typename T>
	Stack<T>::Stack(const Stack<T>& s) : _i(s._i)
	{
		while(_i._i < _i._c)
		{
			push(s._v[_i._i]);
			++_i._i;
		}
	}

	template<typename T>
	Stack<T>::~Stack(void)
	{
	}

	template<typename T>
	Stack<T>& Stack<T>::operator=(const Stack<T>& o)
	{
		Stack<T> t(o);
		
		_i.swap(t._i);

		return *this;
	}

	template<typename T>
	size_t Stack<T>::size(void) const
	{
		return _i._i;
	}

	template<typename T>
	void Stack<T>::push(const T& e)
	{
		if(_i._i == _i._c)
		{
			Stack<T> t(_i._c * 2 + 1);

			while(t._i._i < _i._i)
			{
				t.push(_i._v[t._i._i]);
			}

			t.push(e);

			_i.swap(t._i);
		}
		else
		{
			_i.construct(_i._v + _i._i, e);
			++_i._i;
		}
	}

	template<typename T>
	T& Stack<T>::top(void)
	{
		if(_i._i == 0)
		{
			throw "empty stack";
		}

		return _i._v[_i._i - 1];
	}

	template<typename T>
	void Stack<T>::pop(void)
	{
		if(_i._i == 0)
		{
			throw "cannot pop from empty stack";
		}

		--_i._i;
		_i.destroy(_i._v + _i._i);

		if(_i._i < _i._c / 4)
		{
			Stack<T> t(_i._c / 2);

			while(t._i._i < _i._i)
			{
				t.push(_i._v[t._i._i]);
			}

			_i.swap(t._i);
		}
	}

	template<typename T>
	_Stack_impl<T>::_Stack_impl(int c)
		: _v(static_cast<T *>(c > 0 ? operator new(c * sizeof(T)) : NULL)),
		  _c(c), _i(0)
	{
	}

	template<typename T>
	_Stack_impl<T>::~_Stack_impl(void)
	{
		destroy(_v, _v + _i);
		operator delete(_v);
	}

	template<typename T>
	template<typename TT>
	void _Stack_impl<T>::swap(TT& t1, TT& t2) throw()
	{
		TT t(t1);
		t1 = t2;
		t2 = t;
	}

	template<typename T>
	void _Stack_impl<T>::swap(_Stack_impl<T>& o) throw()
	{
		swap(_v, o._v);
		swap(_i, o._i);
		swap(_c, o._c);
	}

	template<typename T>
	template<typename I>
	void _Stack_impl<T>::construct(I p, const T& t)
	{
		new(&*p) T(t);
	}

	template<typename T>
	template<typename I>
	void _Stack_impl<T>::destroy(I p)
	{
		p->~T();
	}

	template<typename T>
	template<typename I>
	void _Stack_impl<T>::destroy(I p1, I p2)
	{
		while(p1 != p2)
		{
			destroy(p1);
			++p1;
		}
	}
}

#endif

