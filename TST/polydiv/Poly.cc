#include <iostream>
#include <sstream>
#include <cassert>
#include "Poly.h"

struct Poly::Impl
{
	std::vector<double> coef_;
};

Poly::Poly(const std::string& term) : impl_(new Impl)
{
	std::istringstream iss(term);
	while(iss&&!iss.eof())
	{
		double c(0.0);
		iss >> c;
		impl_->coef_.push_back(c);
	}
}

Poly::~Poly(void)
{
	delete impl_;
}

void Poly::print(std::ostream& os) const
{
	assert(impl_&&!impl_->coef_.empty());

	for(size_t i = impl_->coef_.size() ; i ; --i)
	{
		double c(impl_->coef_.at(i - 1));
		if(c == 0.0) continue;
		if(c < 0) { os << "-"; c = -c; }
			else if(i < impl_->coef_.size()) os << "+";
		if(c != 1.0 || i <= 1) os << c;
		if(i > 1)  os << "x";
		if(i > 2) os << "^" << (i - 1);
	}
}

double Poly::doDivision(const Poly& p1, const Poly& p2, std::vector<Poly>& v)
{
	return 0.0;
}

// # ===========================================================================

Poly::Poly(void) : impl_(NULL)
{
}

Poly::Poly(const Poly& p) : impl_(p.impl_ ? new Impl(*p.impl_) : NULL)
{
}

Poly::Poly(Poly&& p) : impl_(NULL)
{
	swap(p);
}

Poly& Poly::operator=(const Poly& p)
{
	Poly t(p);
	swap(t);
	return *this;
}

Poly& Poly::operator=(Poly&& p)
{
	swap(p);
	return *this;
}

void Poly::swap(Poly& p) /*throw()*/
{
	Impl *i = impl_;
	impl_ = p.impl_;
	p.impl_ = i;
}

