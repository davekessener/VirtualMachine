#ifndef VECTOR_H
#define VECTOR_H

#define VECTOR_DEF_SIZE 16

template<class T>
class Vector
{
	public:
		Vector(int = VECTOR_DEF_SIZE);
		Vector(const Vector&);
		~Vector( );
		int Length( ) const;
		bool hasValue(const T&) const;
		Vector<T>& push(T *);
		Vector<T>& push(const T&);
		T& operator[](int);
		const T& operator[](int) const;
	private:
		int resize( );
		int length, index;
		T *vals;
};

#include "Vector.cpp"

#endif

