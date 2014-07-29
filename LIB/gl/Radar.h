#ifndef RADAR_H
#define RADAR_H

#include "inc.h"

class Radar
{
	public:
		Radar( ) { }
		Radar(float, float, float, float);
		void setCam(const vec&, const vec&, const vec&);
		inline void updateCam(const vec& p) { setCam(p, view_, up_); }
		bool isInside(const vec&) const;
		bool isInside(const vec&, float) const;
		const vec& getPos( ) const { return cc_; }
		const vec& getView( ) const { return view_; }
		const vec& getUp( ) const { return up_; }
		inline float getMaxDist( ) const { return dist_; }
	private:
		vec cc_, X_, Y_, Z_;
		float angle_, ratio_, nearD_, farD_;
		float tang_, height_, width_, facX_, facY_;
		vec view_, up_;
		float dist_;
};

#endif

