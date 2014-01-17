#ifndef B_HPP
#define B_HPP

#include "A.hpp"

template<class T>
class B : public A<T>
{
	public:
		inline B(const A<T>& a) : A<T>(a) {}
		void print( );
};

template<class T>
void B<T>::print(void)
{
	std::cout << "Printing from B ... ";
	A<T>::print();
}

#endif

