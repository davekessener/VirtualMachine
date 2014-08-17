#ifndef POOL_H
#define POOL_H

#include <map>

// # ===========================================================================

template<typename T>
class PoolSimple
{
	public:
		typedef T *return_type;

		PoolSimple( ) : elem(NULL) { }
		PoolSimple(return_type r) : elem(r) { use(); }
		virtual ~PoolSimple( ) { }
		static bool isValid(return_type v) { return v != invalid(); }
		static return_type invalid( ) { return NULL; }
		static return_type create( ) { return new T; }
		virtual void destroy( ) { delete elem; }
		virtual void use( ) { }
		virtual bool good( ) { return true; }
		return_type getValue( ) { return elem; }
	protected:
	private:
		return_type elem;
};

// # ---------------------------------------------------------------------------

template<typename T>
class PoolRemote : public virtual PoolSimple<T>
{
	public:
		typedef typename PoolSimple<T>::return_type return_type;

		PoolRemote( ) { }
		PoolRemote(return_type v) : PoolSimple<T>(v) { }
		static return_type create( ) { return PoolSimple<T>::invalid(); }
		virtual void destroy( ) { }
};

// # ---------------------------------------------------------------------------

template<typename T, int SIZE>
class PoolCounted : public virtual PoolSimple<T>
{
	public:
		typedef typename PoolSimple<T>::return_type return_type;

		PoolCounted( ) { }
		PoolCounted(return_type v) : PoolSimple<T>(v) { }
		virtual void use( ) { id = cID++; }
		virtual bool good( ) { return cID - id <= SIZE; }
	private:
		int id;
		static int cID;
};

template<typename T, int SIZE>
int PoolCounted<T, SIZE>::cID = 0;

// # ===========================================================================

template
<
	typename VAL,
	typename KEY = int,
	template<typename> class C = PoolSimple
>
class Pool
{
	public:
		virtual ~Pool( );
		typename C<VAL>::return_type getFromPool(const KEY&);
		void addToPool(const KEY&, typename C<VAL>::return_type);
	private:
		std::map<KEY, C<VAL>> pool;
};

// # ---------------------------------------------------------------------------

template
<
	typename VAL,
	typename KEY,
	template<typename> class C
>
Pool<VAL, KEY, C>::~Pool(void)
{
	for(auto i = pool.begin() ; i != pool.end() ; ++i)
	{
		i->second.destroy();
	}
	pool.clear();
}

template
<
	typename VAL,
	typename KEY,
	template<typename> class C
>
typename C<VAL>::return_type Pool<VAL, KEY, C>::getFromPool(const KEY& key)
{
	if(pool.count(key) == 0)
	{
		typename C<VAL>::return_type v = C<VAL>::create();

		if(C<VAL>::isValid(v))
		{
			pool[key] = C<VAL>(v);
		}
		else
		{
			return C<VAL>::invalid();
		}
	}
	else
	{
		pool.at(key).use();
	}

	return pool.at(key).getValue();
}

template
<
	typename VAL,
	typename KEY,
	template<typename> class C
>
void Pool<VAL, KEY, C>::addToPool(const KEY& key, typename C<VAL>::return_type v)
{
	if(C<VAL>::isValid(v))
	{
		pool[key] = C<VAL>(v);

		for(auto i = pool.begin() ; i != pool.end() ; ++i)
		{
			if(!i->second.good())
			{
				pool.erase(i);
				i = pool.begin();
			}
		}
	}
}

// # ===========================================================================

#endif

