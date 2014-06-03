#ifndef IMPL_PTR_H
#define IMPL_PTR_H

namespace lisp
{
	template<typename T>
	class impl_ptr
	{
		public:
			impl_ptr( );
			impl_ptr(T *);
			impl_ptr(const impl_ptr<T>&);
			impl_ptr(impl_ptr<T>&&);
			~impl_ptr( ) throw();
			T& operator*( ) const;
			T *operator->( ) const;
			operator T *( ) const;
			explicit operator bool( ) const;
			void swap(impl_ptr<T>&) throw();
		private:
			impl_ptr<T>& operator=(const impl_ptr<T>&);

			mutable T *internal_;
	};

	template<typename T>
	impl_ptr<T>::impl_ptr(void) : internal_(new T)
	{
	}

	template<typename T>
	impl_ptr<T>::impl_ptr(T *t) : internal_(t)
	{
	}

	template<typename T>
	impl_ptr<T>::impl_ptr(const impl_ptr<T>& i) : internal_(i.internal_ ? new T(*i) : NULL)
	{
	}

	template<typename T>
	impl_ptr<T>::impl_ptr(impl_ptr<T>&& i) : internal_(NULL)
	{
		swap(i);
	}

	template<typename T>
	impl_ptr<T>::~impl_ptr(void) throw()
	{
		delete internal_;
	}

	template<typename T>
	T& impl_ptr<T>::operator*(void) const
	{
		return *internal_;
	}

	template<typename T>
	T *impl_ptr<T>::operator->(void) const
	{
		return internal_;
	}

	template<typename T>
	impl_ptr<T>::operator T *(void) const
	{
		return internal_;
	}

	template<typename T>
	impl_ptr<T>::operator bool(void) const
	{
		return internal_ != NULL;
	}

	template<typename T>
	void impl_ptr<T>::swap(impl_ptr<T>& i) throw()
	{
		internal_ ^= i.internal_;
		i.internal_ ^= internal_;
		internal_ ^= i.internal_;
	}
}

#endif

