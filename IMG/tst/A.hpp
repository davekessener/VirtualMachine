#ifndef A_HPP
#define A_HPP

#include <iostream>

template<int N>
class A
{
	public:
		A(double);
		A( );
		A(const A<N>&);
		~A( );
		double percentage( ) const;
		A& operator=(const A<N>&);
	private:
		double pct;
		static const int SQR = N * N;
};

template<int N>
A<N>::A(double _pct) : pct(_pct)
{
	std::cout << "Initializing " << percentage() << "% of " << SQR << "!" << std::endl;
}

template<int N>
A<N>::A(void) : pct(0.0)
{
	std::cout << "Initializing default for " << SQR << "!" << std::endl;
}

template<int N>
A<N>::A(const A<N>& a) : pct(a.pct)
{
	std::cout << "Initializing copy of " << percentage() << "% of " << SQR << "!" << std::endl;
}

template<int N>
A<N>::~A(void)
{
	std::cout << "Finalizing " << percentage() << "% of " << SQR << "!" << std::endl;
}

template<int N>
double A<N>::percentage(void) const
{
	return (((int) (pct * 10000.0 + 0.5)) / 100.0);
}

template<int N>
A<N>& A<N>::operator=(const A<N>& a)
{
	std::cout << "Assigning " << a.percentage() << "% to " << percentage() << "% of " << SQR << "!" << std::endl;
	pct = a.pct;
}



#endif

