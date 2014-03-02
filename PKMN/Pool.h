#ifndef POOL_H
#define POOL_H

#include <map>

// # ===========================================================================

template<typename T>
class PoolRetainerNew
{
	public:
		typedef T *return_type;
		PoolRetainerNew( ) : ptr(new T()) { }
		virtual ~PoolRetainerNew( ) { }
		return_type getReturn( ) { return ptr; }
	protected:
		void free( ) { delete ptr; ptr = NULL; }
	private:
		return_type ptr;
};

// # ===========================================================================

template<typename T>
class PoolValidationConst : public T
{
	public:
		template<typename TT>
			static void order(const TT&, std::map<TT, PoolValidationConst<T>>&);
	protected:
	private:
		T elem;
};

template<typename T>
template<typename TT>
void PoolValidationConst<T>::order(const TT& idx, std::map<TT, PoolValidationConst<T>>& map)
{
}

// # ===========================================================================

template<typename T, int AGE>
class PoolValidationAge : public T
{
	public:
		PoolValidationAge( ) : T(), age(-1) { }
		template<typename TT>
			static void order(const TT&, std::map<TT, PoolValidationAge<T, AGE>>&);
	protected:
	private:
		int age;
};

template<typename T, int AGE>
template<typename TT>
void PoolValidationAge<T, AGE>::order(const TT& idx, std::map<TT, PoolValidationAge<T, AGE>>& map)
{
	int age = map.at(idx).age;

	for(auto i = map.begin() ; i != map.end() ; ++i)
	{
		if(age < 0 || i->second.age < age) ++(i->second.age);
	}

	map.at(idx).age = 0;

	for(auto i = map.begin() ; i != map.end() ; ++i)
	{
		if(i->second.age >= AGE)
		{
			i->second.T::free();
			map.erase(i);
			i = map.begin();
		}
	}
}

// # ===========================================================================

template
<
	typename T,
	typename TT = int,
	template<typename> class C = PoolValidationConst
>
class Pool
{
	public:
		typename C<T>::return_type getFromPool(const TT&);
	private:
		std::map<TT, C<T>> pool;
};

template
<
	typename T,
	typename TT,
	template<typename> class C
>
typename C<T>::return_type Pool<T, TT, C>::getFromPool(const TT& idx)
{
	if(pool.count(idx) == 0)
	{
		pool[idx] = C<T>{};
	}

	C<T>::order(idx, pool);

	return pool.at(idx).getReturn();
}

// # ===========================================================================

#endif

