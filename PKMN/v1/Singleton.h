#ifndef SINGLETON_H
#define SINGLETON_H

template<typename T>
class Singleton
{
	public:
		static T& instance( );
	protected:
	private:
		Singleton( );
		Singleton(const Singleton<T>&);
		Singleton<T>& operator=(const Singleton<T>&);
};

template<typename T>
T& Singleton<T>::instance(void)
{
	static T t;
	return t;
}

#endif

