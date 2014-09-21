#include <iostream>
#include <sstream>
#include <iterator>
#include <vector>
#include <memory>

template<typename T>
size_t strlen(const T *s) { const T *t(s); while(*t) ++t; return t - s; }

template<typename S, typename I>
void fprintfv(S& s, I i1, I i2)
{
	while(i1 != i2)
	{
		if(*i1 == '%')
		{
			if(++i1 == i2) throw std::string("fprintfv ends on '%'");
			else if(*i1 != '%') throw std::string("fprintfv encountered '%' without any parameters!");
		}

		s << *i1; ++i1;
	}
}

template<typename S, typename I, typename T, typename ... TT>
void fprintfv(S& s, I i1, I i2, const T& t, const TT& ... tt)
{
	while(i1 != i2)
	{
		if(*i1 == '%' && (++i1 == i2 || *i1 != '%'))
		{
			s << t;
			fprintfv(s, i1, i2, tt...);
			return;
		}

		s << *i1; ++i1;
	}

	throw std::string("fprintf ends prematurely");
}

template<typename S, typename T, typename ... TT>
void fprintf(S& s, const T& t, const TT& ... tt)
{
	fprintfv(s, t.cbegin(), t.cend(), tt...);
}

template<typename S, typename T, typename ... TT>
void fprintf(S& s, const T *t, const TT& ... tt)
{
	fprintfv(s, t, t + strlen(t), tt...);
}

template<typename T, typename ... TT>
void printf(const T& t, const TT& ... tt)
{
	fprintf(std::cout, t, tt...);
}

template<typename T, typename ... TT>
void printf(const T *t, const TT& ... tt)
{
	fprintf(std::cout, t, tt...);
}

template<typename T, typename ... TT>
std::string sprintf(const T& t, const TT& ... tt)
{
	std::istringstream iss;
	fprintf(iss, t, tt...);
	return iss.str();
}

template<typename T, typename ... TT>
std::string sprintf(const T *t, const TT& ... tt)
{
	std::istringstream iss;
	fprintf(iss, t, tt...);
	return iss.str();
}

template<typename S>
void stringifyImpl(S&) { }

template<typename S, typename T, typename ... TT>
void stringifyImpl(S& s, const T& t, const TT& ... tt)
{
	s << t;
	stringifyImpl(s, tt...);
}

template<typename ... TT>
std::string stringify(const TT& ... tt)
{
	std::ostringstream oss;
	stringifyImpl(oss, tt...);
	return oss.str();
}

int main(int argc, char *argv[])
try
{
	std::string s(stringify("error @", 5, "yout pieceof ", 0.33, " by over ", 9000));
	std::cout << s << std::endl;
	return 0;
}
catch(const std::string& e)
{
	std::cerr << std::endl << "ERR: " << e << std::endl;
	return 1;
}

