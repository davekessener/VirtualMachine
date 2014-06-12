#ifndef SELECTOR_H
#define SELECTOR_H

#include <functional>

template<typename T>
class Selector
{
	typedef std::function<T(void)> getter_fn;
	typedef std::function<void(T)> setter_fn;

	public:
		Selector(getter_fn, setter_fn);
		operator T( );
		T operator=(const T&);
	private:
		getter_fn _get;
		setter_fn _set;
};

template<typename T>
Selector<T>::Selector(getter_fn get, setter_fn set) : _get(get), _set(set)
{
}

template<typename T>
Selector<T>::operator T(void)
{
	return _get();
}

template<typename T>
T Selector<T>::operator=(const T& t)
{
	_set(t);
	return _get();
}

#endif

