#include <iostream>

template<class T>
class arr
{
	public:
		arr(int);
		arr(const arr<T>&);
		~arr( );
		arr<T>& add(const arr<T>&);
		bool compare(const T&);
	protected:
		inline int getSize() const { return s; }
	private:
		T *d;
		int s;
		static const size_t ELEM_S = sizeof(T);
};

class iarr : public arr<int>
{
	public:
		iarr(int);
		iarr(const iarr&);
		operator int() const;
	private:
		int os;
		arr<double> darr;
};

iarr::operator int(void) const
{
	std::cout << "Conversion to 'int' == " << getSize() << "." << std::endl;
	return getSize();
}

iarr::iarr(int os) : arr<int>(os), os((os % 4) * 3), darr(this->os)
{
}

iarr::iarr(const iarr& riarr) : arr<int>(riarr), darr(23)
{
}

int main(int argc, char *argv[])
{
	arr<int> *a = new arr<int>(7);
	arr<int> a2(42);
	iarr a3(17);
	iarr a4(a3);

	int size = a3;

	a->add(a2);
	std::cout << "Comparison is " << a->compare(7) << "." << std::endl;
	a3.add(a2);

	delete a;

	return 0;
}

template<class T>
arr<T>::arr(int s) : s(s)
{
	d = new T[s];
	std::cout << "Created array of size " << s << "!" << std::endl;
	std::cout << "[ Element size is " << ELEM_S << ". ]" << std::endl;
}

template<class T>
arr<T>::arr(const arr<T>& rarr) : s(rarr.s)
{
	d = new T[s];
	std::cout << "Copied array of size " << s << "!" << std::endl;
	std::cout << "[ Element size is still " << ELEM_S << ". ]" << std::endl;
}

template<class T>
arr<T>::~arr(void)
{
	delete[] d;
	std::cout << "Deleted array of size " << s << "!" << std::endl;
}

template<class T>
arr<T>& arr<T>::add(const arr<T>& toAdd)
{
	std::cout << "Adding array of size " << toAdd.s << " to array of size " << s << "." << std::endl;

	return *this;
}

template<class T>
bool arr<T>::compare(const T& t)
{
	return t == T(s);
}

