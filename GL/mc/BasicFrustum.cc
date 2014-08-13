//#include <iostream>
#include "BasicFrustum.h"

#define ANG2RAD 3.14159265358979323846/180.0

BasicFrustum::BasicFrustum(float angle, float ratio, float nearD, float farD)
	: angle_(angle), ratio_(ratio), nearD_(nearD), farD_(farD), tang_(tan(ANG2RAD * angle * 0.5)),
	  nh_(nearD * tang_), nw_(nh_ * ratio), fh_(farD * tang_), fw_(fh_ * ratio),
	  dist_(mag(vec(nh_/2, nw_/2, farD)))
{
}

void BasicFrustum::setCam(const vec& p, const vec& v, const vec& u)
{
	vec dir, nc, fc, X, Y, Z;

	pos_ = p;
	view_ = v;
	up_ = u;

	Z = -v;
	X = u ^ Z;
	Y = Z ^ X;

	nc = p - (Z * nearD_);
	fc = p - (Z * farD_);

	pl_[NEAR] = plane(-Z, nc);
	pl_[FAR]  = plane( Z, fc);

	vec aux, normal;

	aux = normalize((nc + (Y * nh_)) - p);
	normal = aux ^ X;
	pl_[TOP] = plane(normal, nc + (Y * nh_));

	aux = normalize((nc - (Y * nh_)) - p);
	normal = X ^ aux;
	pl_[BOTTOM] = plane(normal, nc - (Y * nh_));

	aux = normalize((nc - (X * nw_)) - p);
	normal = aux ^ Y;
	pl_[LEFT] = plane(normal, nc - (X * nw_));

	aux = normalize((nc + (X * nw_)) - p);
	normal = Y ^ aux;
	pl_[RIGHT] = plane(normal, nc + (X * nw_));

//	cor_[0].p[0] = nc + Y * nh_ - X * nw_;
//	cor_[0].p[1] = nc + Y * nh_ + X * nw_;
//	cor_[0].p[2] = nc - Y * nh_ - X * nw_;
//	cor_[0].p[3] = nc - Y * nh_ + X * nw_;
//
//	cor_[1].p[0] = fc + Y * fh_ - X * fw_;
//	cor_[1].p[1] = fc + Y * fh_ + X * fw_;
//	cor_[1].p[2] = fc - Y * fh_ - X * fw_;
//	cor_[1].p[3] = fc - Y * fh_ + X * fw_;
}

bool BasicFrustum::isInside(const vec& c, float r) const
{
	float d;

	for(int i = 0 ; i < 6 ; ++i)
	{
		d = pl_[i] - c;
		if(d < -r) return false;
	}

	return true;
}

//void BasicFrustum::print(std::ostream& os) const
//{
//	for(int i = 0 ; i < 2 ; ++i)
//	{
//		if(i) os << "\n";
//		os << i << ": [";
//		for(int j = 0 ; j < 4 ; ++j)
//		{
//			if(j) os << ", ";
//			os << "{" << cor_[i].p[j].dx << ", " << cor_[i].p[j].dy << ", " << cor_[i].p[j].dz << "}";
//		}
//		os << "]";
//	}
//}

