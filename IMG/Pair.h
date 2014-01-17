#ifndef PAIR_H
#define PAIR_H

template<class T1, class T2>
class Pair
{
	public:
		inline Pair( ) : _first(NULL), _second(NULL) {};
		inline Pair(T1& __first, T2& __second) : _first(&__first), _second(&__second) {};
		inline Pair(const Pair<T1, T2>& pair) : _first(pair._first), _second(pair._second) {};
		inline T1& first( ) { return *_first; }
		inline const T1& first( ) const { return *_first; }
		inline T2& second( ) { return *_second; }
		inline const T2& second( ) const { return *_second; }
		Pair& operator=(const Pair<T1, T2>& p);
	private:
		T1 *_first;
		T2 *_second;
};

template<class T1, class T2>
Pair<T1, T2>& Pair<T1, T2>::operator=(const Pair<T1, T2>& p)
{
	_first = p._first ? new T1(*p._first) : NULL;
	_second = p._second;
	return *this;
}

#endif

