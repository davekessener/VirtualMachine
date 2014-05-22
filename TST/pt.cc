#include <iostream>

void printTest(void)
{
}

template<typename T, typename ... TT>
void printTest(const T *t, const TT * ... tt)
{
	std::cout << *t << std::endl;
	printTest(tt...);
}

int main(int argc, char *argv[])
{
	const char *s = "hello world";
	int i = 42; float f = 3.141;
	printTest(&argc, &i, &s, &f);

	const std::string arr[3] = {"hello", "there,", "handsome!"};

	for(i = 0 ; i < 3 ; ++i)
	{
		std::cout << arr[i] << std::endl;
	}

	return 0;
}

