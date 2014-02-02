#ifndef SINGLETON_HPP
#define SINGLETON_HPP

#include <stdint.h>

template<class T>
class Singleton
{
	public:
		static T& Instance(void);
	private:
		Singleton( );
		~Singleton( );
};

template<class T>
Singleton<T>::Singleton(void)
{
	std::cout << "Singleton constructor." << std::endl;
}

template<class T>
Singleton<T>::~Singleton(void)
{
	std::cout << "Singleton destructor." << std::endl;
}

template<class T>
T& Singleton<T>::Instance(void)
{
	static T instance;

	return instance;
}

#endif

