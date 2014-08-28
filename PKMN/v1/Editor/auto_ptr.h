#ifndef EDITOR_AUTOPTR_H
#define EDITOR_AUTOPTR_H

namespace editor
{
	template<typename T>
	class auto_ptr
	{
		public:
			auto_ptr( );
			auto_ptr(T *);
			auto_ptr(auto_ptr<T>&);
			auto_ptr(auto_ptr<T>&&);
			~auto_ptr( );
			auto_ptr<T>& operator=(auto_ptr<T>&);
			auto_ptr<T>& operator=(auto_ptr<T>&&);
			T& operator*( );
			const T& operator*( ) const;
			T *operator->( );
			const T *operator->( ) const;
			explicit operator bool( ) const;
			void swap(auto_ptr<T>&) throw();
		private:
			T *t_;
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
	auto_ptr<T>::auto_ptr(auto_ptr<T>& p) : t_(p.t_)
	{
		p.t_ = NULL;
	}

	template<typename T>
	auto_ptr<T>::auto_ptr(auto_ptr<T>&& p) : t_(p.t_)
	{
		p.t_ = NULL;
	}

	template<typename T>
	auto_ptr<T>::~auto_ptr(void)
	{
		if(t_) delete t_;
		t_ = NULL;
	}

	template<typename T>
	auto_ptr<T>& auto_ptr<T>::operator=(auto_ptr<T>& p)
	{
		auto_ptr<T> t(p);
		swap(t);
	}

	template<typename T>
	auto_ptr<T>& auto_ptr<T>::operator=(auto_ptr<T>&& p)
	{
		auto_ptr<T> t(p);
		swap(t);
	}

	template<typename T>
	T& auto_ptr<T>::operator *(void)
	{
		assert(t_);
		return *t_;
	}

	template<typename T>
	const T& auto_ptr<T>::operator *(void) const
	{
		assert(t_);
		return *t_;
	}

	template<typename T>
	T *auto_ptr<T>::operator->(void)
	{
		assert(t_);
		return t_;
	}

	template<typename T>
	const T *auto_ptr<T>::operator->(void) const
	{
		assert(t_);
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

