#ifndef POLY_H
#define POLY_H

#include <string>
#include <vector>
#include <iosfwd>

class Poly
{
	public:
		Poly( );
		Poly(const Poly&);
		Poly(Poly&&);
		explicit Poly(const std::string&);
		~Poly( );
		Poly& operator=(const Poly&);
		Poly& operator=(Poly&&);
		void print(std::ostream&) const;
		void swap(Poly&) /*throw()*/;
		static double doDivision(const Poly&, const Poly&, std::vector<Poly>&);
	private:
		struct Impl;
		Impl *impl_;
};

inline std::vector<Poly> operator/(const Poly& p1, const Poly& p2)
{
	std::vector<Poly> v;
	Poly::doDivision(p1, p2, v);
	return v;
}

inline std::ostream& operator<<(std::ostream& os, const Poly& p)
{
	p.print(os);
	return os;
}

#endif

