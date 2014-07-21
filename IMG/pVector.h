#ifndef PVECTOR_H
#define PVECTOR_H

#include <iostream>
#include <string.h>

#define PVEC_DEF_LEN 16

template<class T>
class pVector
{
	public:
		pVector(int = PVEC_DEF_LEN);
		~pVector( );
		pVector<T>& push(T *);
		T *poll( );
		T *pop( );
		T *operator[](int);
		const T *operator[](int) const;
		pVector<T>& remove(T *);
		int Length( ) const;
		bool Empty( ) const;
		std::string toString( ) const;
	private:
		void resize( );
		int idx_a, idx_e, length;
		T **_v;
};

template<class T>
pVector<T>::pVector(int _length) : idx_a(0), idx_e(0), length(_length <= 0 ? PVEC_DEF_LEN : _length)
{
	//std::cout << "pVector<T>::pVector(int = " << _length << ");" << std::endl;
	_v = new T*[length];
}

template<class T>
pVector<T>::~pVector(void)
{
	//std::cout << "pVector<T>::~pVector(void);" << std::endl;
	delete[] _v;
}

template<class T>
pVector<T>& pVector<T>::push(T *t)
{
	_v[idx_e++] = t;

	resize();

	//std::cout << "pVector<T>::push(T* = ";
	//printf("%p);\n", t);
}

template<class T>
T *pVector<T>::poll(void)
{
	T *t = _v[idx_a++];

	//printf("pVector<T>::poll(); = %p\n", t);

	resize();

	return t;
}

template<class T>
T *pVector<T>::pop(void)
{
	T *t = _v[--idx_e];

	//printf("pVector<T>::pop(); = %p\n", t);

	resize();

	return t;
}

template<class T>
T *pVector<T>::operator[](int idx)
{
	return _v[idx_a + idx];
}

template<class T>
const T *pVector<T>::operator[](int idx) const
{
	return _v[idx_a + idx];
}

template<class T>
pVector<T>& pVector<T>::remove(T *t)
{
	for(int i = idx_a ; i < idx_e ; i++)
	{
		if(t == _v[i])
		{
			for(int j = i ; j < idx_e - 1 ; j++)
			{
				_v[j] = _v[j + 1];
			}

			idx_e--;
		}
	}
}

template<class T>
int pVector<T>::Length(void) const
{
	return idx_e - idx_a;
}

template<class T>
bool pVector<T>::Empty(void) const
{
	return idx_a == idx_e;
}

template<class T>
std::string pVector<T>::toString(void) const
{
	char buf[256];

	sprintf(buf, "pVector<T>@%p: idx_a=%i, idx_e=%i, length=%i", this, idx_a, idx_e, length);

	return std::string(buf);
}

// # ---------------------------------------------------------------------------

template<class T>
void pVector<T>::resize(void)
{
	//std::cout << "pVector<T>::resize();";

	if(idx_a == idx_e && idx_a > 0)
	{
		idx_a = idx_e = 0;
	}
	else if(idx_e == length)
	{
		length *= 2;

		T **tmp = _v;
		_v = new T*[length];

		memcpy(_v + idx_a, tmp + idx_a, (idx_e - idx_a) * sizeof(T *));
		
		//std::cout << " to " << length;

		delete[] tmp;
	}

	while(length > PVEC_DEF_LEN)
	{
		if(idx_e < length / 4)
		{
			length /= 2;

			T **tmp = _v;
			_v = new T*[length];

			memcpy(_v + idx_a, tmp + idx_a, (idx_e - idx_a) * sizeof(T *));

			delete[] tmp;
		}
		else if(idx_a > length / 2)
		{
			idx_a -= length / 2;
			idx_e -= length / 2;

			T **tmp = _v;
			_v = new T*[length - length / 2];

			memcpy(_v + idx_a, tmp + length / 2 + idx_a, (idx_e - idx_a) * sizeof(T *));

			delete[] tmp;

			length -= length / 2;

			//std::cout << " to " << length;
		}
		else
		{
			break;
		}
	}

	//std::cout << std::endl;
}

#endif

