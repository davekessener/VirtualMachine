template<class T>
Vector<T>::Vector(int size) : length(size <= 0 ? VECTOR_DEF_SIZE : size), index(0)
{
	vals = new T[length];
}

template<class T>
Vector<T>::Vector(const Vector<T>& v) : length(v.length), index(v.index)
{
	vals = new T[length];
	for(int i = 0 ; i < index ; i++)
	{
		vals[i] = v.vals[i];
	}
}

template<class T>
Vector<T>::~Vector(void)
{
	delete[] vals;
}

template<class T>
int Vector<T>::Length(void) const
{
	return index;
}

template<class T>
bool Vector<T>::hasValue(const T& t) const
{
	for(int i = 0 ; i < index ; i++)
	{
		if(vals[i] == t) return true;
	}

	return false;
}

template<class T>
Vector<T>& Vector<T>::push(T *pT)
{
	vals[resize()] = *pT;
	return *this;
}

template<class T>
Vector<T>& Vector<T>::push(const T& t)
{
	vals[resize()] = T(t);
	return *this;
}

template<class T>
T& Vector<T>::operator[](int idx)
{
#ifdef DEBUG
	if(idx < 0 || idx >= index)
	{
		std::cout << "ERR: Accessed vector(" << index << "/" << length << ") @" << idx << std::endl;
	}
#endif

	return vals[idx];
}

template<class T>
const T& Vector<T>::operator[](int idx) const
{
#ifdef DEBUG
	if(idx < 0 || idx >= index)
	{
		std::cout << "ERR: Accessed vector(" << index << "/" << length << ") @" << idx << std::endl;
	}
#endif

	return vals[idx];
}

template<class T>
int Vector<T>::resize(void)
{
	if(++index == length)
	{
		length *= 2;

		T *tmp = vals;
		vals = new T[length];

		for(int i = 0 ; i < index - 1; i++)
		{
			vals[i] = tmp[i];
		}

		delete[] tmp;
	}

	return index - 1;
}

