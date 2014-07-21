#ifndef QUEUE_H
#define QUEUE_H

#include <iostream>

#define QUEUE_DEF_SIZE 16

template<class T>
class Queue
{
	public:
		Queue(int = QUEUE_DEF_SIZE);
		Queue(const Queue<T>&);
		~Queue( );
		Queue<T>& push(T *);
		T *poll( );
		int Size( ) const;
		bool isEmpty( ) const;
	private:
		int resize( );
		T **_vs;
		int l, idx_a, idx_e;
};

#include "Queue.cpp"

#endif

