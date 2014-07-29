#include "Radar.h"

#define ANG2RAD 3.14159265358979323846/180.0

Radar::Radar(float angle, float ratio, float nearD, float farD)
	: angle_(angle), ratio_(ratio), nearD_(nearD), farD_(farD),
	  tang_((float)tan(ANG2RAD * angle * 0.5)),
	  height_(nearD * tang_), width_(height_ * ratio),
	  facX_(1.0/cos(atan(tang_*ratio))), facY_(1.0/cos(ANG2RAD*angle*0.5)),
	  dist_(mag(vec(height_/2, width_/2, farD)))
{
}

void Radar::setCam(const vec& p, const vec& v, const vec& u)
{
	cc_ = p;
	view_ = v;
	up_ = u;

	Z_ = -v;
	X_ = Z_ ^ u;
	Y_ = X_ ^ Z_;
}

bool Radar::isInside(const vec& p) const
{
	float pcx, pcy, pcz, aux;

	vec v(p - cc_);

	pcz = v * -Z_;
	if(pcz > farD_ || pcz < nearD_) return false;

	pcy = v * Y_;
	aux = pcz * tang_;
	if(pcy > aux || pcy < -aux) return false;

	pcx = v * X_;
	aux *= ratio_;
	if(pcx > aux || pcx < -aux) return false;

	return true;
}

bool Radar::isInside(const vec& p, float r) const
{
	float d, ax, ay, az;

	vec v(p - cc_);

	az = v * -Z_;
	if(az > farD_ + r || az < nearD_ - r) return false;
	
	ay = v * Y_;
	d = facY_ * r;
	az *= tang_;
	if(ay > az + d || ay < -az - d) return false;

	ax = v * X_;
	d = facX_ * r;
	az *= ratio_;
	if(ax > az + d || ax < -az - d) return false;

	return true;
}

