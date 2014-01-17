template<class T>
Map<T>::Map(int size) : index(0), length(size <= 0 ? MAP_DEF_LEN : size)
{
	keyvals = new Pair<KEY, T>[length];
}

template<class T>
Map<T>::Map(const Map<T>& map) : index(map.index), length(map.length)
{
	keyvals = new Pair<KEY, T>[length];

	for(int i = 0 ; i < length ; i++)
	{
		keyvals[i] = map.keyvals[i];
	}
}

template<class T>
Map<T>::~Map(void)
{
	delete[] keyvals;
}

template<class T>
int Map<T>::Length(void) const
{
	return index;
}

template<class T>
bool Map<T>::hasKey(KEY key) const
{
	return getKeyIndex(key) >= 0;
}

template<class T>
bool Map<T>::hasValue(const T& value) const
{
	for(int i = 0 ; i < index ; i++)
	{
		if(keyvals[i].second() == value) return true;
	}

	return false;
}

template<class T>
Map<T>& Map<T>::insert(KEY key, T *value)
{
	int idx = getKeyIndex(key);

	if(idx < 0)
	{
		idx = resize();
	}

	keyvals[idx] = Pair<KEY, T>(key, *value);

	return *this;
}

template<class T>
Map<T>& Map<T>::insert(KEY key, const T& value)
{
	int idx = getKeyIndex(key);

	if(idx < 0)
	{
		idx = resize();
	}

	keyvals[idx] = Pair<KEY, T>(key, T(value));

	return *this;
}

template<class T>
T& Map<T>::operator[](KEY key)
{
	int idx = getKeyIndex(key);

	if(idx < 0) idx = resize();

	return keyvals[idx].second();
}

template<class T>
const T& Map<T>::operator[](KEY key) const
{
	return keyvals[getKeyIndex(key)].second();
}

template<class T>
Map<T>& Map<T>::operator=(const Map<T>& map)
{
	length = map.length;
	index = map.index;

	keyvals = new Pair<KEY, T>[length];
	for(int i = 0 ; i < index ; i++)
	{
		keyvals[i] = map.keyvals[i];
	}
}

// # ---------------------------------------------------------------------------

template<class T>
int Map<T>::getKeyIndex(KEY key) const
{
	for(int i = 0 ; i < index ; i++)
	{
		if(keyvals[i].first() == key) return i;
	}

	return -1;
}

template<class T>
int Map<T>::resize(void)
{
	if(++index == length)
	{
		length *= 2;

		Pair<KEY, T> *tmp = keyvals;
		keyvals = new Pair<KEY, T>[length];

		for(int i = 0 ; i < index - 1 ; i++)
		{
			keyvals[i] = tmp[i];
		}

		delete[] tmp;
	}

	return index - 1;
}

