#ifndef BASIC_FRUSTUM_H
#define BASIC_FRUSTUM_H

//#include <iosfwd>
#include "inc.h"

class BasicFrustum
{
	private:
		enum
		{
			TOP = 0,
			BOTTOM,
			LEFT,
			RIGHT,
			NEAR,
			FAR
		};
	public:
		BasicFrustum( ) { }
		BasicFrustum(float, float, float, float);
		void setCam(const vec&, const vec&, const vec&);
		inline void updateCam(const vec& p) { setCam(p, view_, up_); }
		bool isInside(const vec&, float) const;
		const vec& getPos( ) const { return pos_; }
		const vec& getView( ) const { return view_; }
		const vec& getUp( ) const { return up_; }
		inline float getMaxDist( ) const { return dist_; }
//		void print(std::ostream&) const;
	private:
		plane pl_[6];
		vec pos_, view_, up_;
		float angle_, ratio_, nearD_, farD_, tang_;
		float nh_, nw_, fh_, fw_;
		float dist_;
};

//inline std::ostream& operator<<(std::ostream& os, const BasicFrustum& f)
//{
//	f.print(os);
//	return os;
//}

#endif

