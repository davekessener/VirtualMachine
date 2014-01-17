#ifndef MAP_H
#define MAP_H

#include <iostream>
#include <stdint.h>
#include "Pair.h"

#define MAP_DEF_LEN 16

template<class T = int>
class Map
{
	public:
		typedef uint64_t KEY;

		Map(int size = MAP_DEF_LEN);
		Map(const Map<T>&);
		~Map( );
		int Length( ) const;
		bool hasKey(KEY key) const;
		bool hasValue(const T& value) const;
		Map& insert(KEY key, T *value);
		Map& insert(KEY key, const T& value);
		T& operator[](KEY);
		const T& operator[](KEY) const;
		Map& operator=(const Map&);
	private:
		int getKeyIndex(KEY key) const;
		int resize( );
		int length, index;
		Pair<KEY, T> *keyvals;
};

#include "Map.cpp"

#endif

