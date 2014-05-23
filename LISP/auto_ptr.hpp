#ifndef LISP_AUTOPTR_H
#define LISP_AUTOPTR_H

namespace lisp
{
	template<typename T>
	class auto_ptr
	{
		public:
			auto_ptr( );
			auto_ptr(T *);
			auto_ptr(auto_ptr<T>&);
			auto_ptr(auto_ptr<T>&&);
			~auto_ptr( ) throw();
			auto_ptr<T>& operator=(auto_ptr<T>);
			T& operator*( ) const;
			T *operator->( ) const;
			operator T *( ) const;
			explicit operator bool( ) const;
			void swap(auto_ptr<T>&) throw();
		private:
			mutable T *t_;
	};

	template<typename T>
	auto_ptr<T>::auto_ptr(void) : t_(NULL)
	{
	}

	template<typename T>
	auto_ptr<T>::auto_ptr(T *t) : t_(t)
	{
	}

	template<typename T>
	auto_ptr<T>::auto_ptr(auto_ptr<T>& p) : t_(NULL)
	{
		swap(p);
	}

	template<typename T>
	auto_ptr<T>::auto_ptr(auto_ptr<T>&& p) : t_(NULL)
	{
		swap(p);
	}

	template<typename T>
	auto_ptr<T>::~auto_ptr(void) throw()
	{
		if(t_) delete t_;
		t_ = NULL;
	}

	template<typename T>
	auto_ptr<T>& auto_ptr<T>::operator=(auto_ptr<T> p)
	{
		swap(p);
	}

	template<typename T>
	T& auto_ptr<T>::operator *(void) const
	{
		return *t_;
	}

	template<typename T>
	T *auto_ptr<T>::operator->(void) const
	{
		return t_;
	}

	template<typename T>
	auto_ptr<T>::operator T *(void) const
	{
		return t_;
	}

	template<typename T>
	auto_ptr<T>::operator bool(void) const
	{
		return t_ != NULL;
	}

	template<typename T>
	void auto_ptr<T>::swap(auto_ptr<T>& p) throw()
	{
		T *t = t_;
		t_ = p.t_;
		p.t_ = t;
	}
}

#endif

