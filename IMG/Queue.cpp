template<class T>
Queue<T>::Queue(int _s) : l(_s), idx_a(0), idx_e(0)
{
	_vs = new T*[l];
}

template<class T>
Queue<T>::Queue(const Queue<T>& q) : l(q.l), idx_a(q.idx_a), idx_e(q.idx_e)
{
	_vs = new T*[l];
	for(int i = idx_a ; i < idx_e ; i++)
	{
		_vs[i] = q._vs[i];
	}
}

template<class T>
Queue<T>::~Queue(void)
{
	delete[] _vs;
}

template<class T>
Queue<T>& Queue<T>::push(T *t)
{
	_vs[resize()] = t;

	return *this;
}

template<class T>
T *Queue<T>::poll(void)
{
#ifdef DEBUG
	if(idx_a >= idx_e)
	{
		std::cerr << "ERR: Trying to poll from empty queue!" << std::endl;
	}
#endif

	return _vs[idx_a++];
}

template<class T>
int Queue<T>::Size(void) const
{
	return idx_e - idx_a;
}

template<class T>
bool Queue<T>::isEmpty(void) const
{
	return idx_a == idx_e;
}

template<class T>
int Queue<T>::resize(void)
{
	if(++idx_e == l)
	{
		l *= 2;
		T **tmp = _vs;
		_vs = new T*[l];

		for(int i = idx_a ; i < idx_e - 1 ; i++)
		{
			_vs[i] = tmp[i];
		}

		delete[] tmp;
	}

	if(l > QUEUE_DEF_SIZE && idx_a > l / 2)
	{
		l /= 2;
		idx_a -= l;
		idx_e -= l;

		T **tmp = _vs;
		_vs = new T*[l];

		for(int i = idx_a ; i < idx_e ; i++)
		{
			_vs[i] = tmp[i + l];
		}

		delete[] tmp;
	}

	return idx_e - 1;
}

