#ifndef A_HPP
#define A_HPP

template<class T>
class A
{
	public:
		inline A(const T& _t) : t(_t) {}
		inline A(const A<T>& a) : t(a.t) {}
		void print( );
		T t;
};

template<class T>
void A<T>::print(void)
{
	uint8_t *p = (uint8_t *) &t;
	
	for(int i = sizeof(T) - 1 ; i >= 0 ; i--)
	{
		printf("%02X%c", p[i], i ? ' ' : '\n');
	}
}

#endif

