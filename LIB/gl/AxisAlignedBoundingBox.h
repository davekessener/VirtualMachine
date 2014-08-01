#ifndef AXISALIGNEDBOUNDINGBOX_H
#define AXISALIGNEDBOUNDINGBOX_H

#include "inc.h"

class AABB
{
	public:
#define min(a,b) (((a)<(b))?(a):(b))
#define max(a,b) (((a)>(b))?(a):(b))
		AABB(float x1 = 0.0, float y1 = 0.0, float z1 = 0.0, float x2 = 0.0, float y2 = 0.0, float z2 = 0.0)
			: min_(min(x1, x2), min(y1, y2), min(z1, z2)), max_(max(x1, x2), max(y1, y2), max(z1, z2)) { }
#undef max
#undef min
		AABB(const vec& p1, const vec& p2) : AABB(p1.dx, p1.dy, p1.dz, p2.dx, p2.dy, p2.dz) { }
		void render(DWORD, DWORD) const;
		vec& min( ) { return min_; }
		vec& max( ) { return max_; }
		int width( ) const { return max_.dx - min_.dx; }
		int height( ) const { return max_.dy - min_.dy; }
		int length( ) const { return max_.dz - min_.dz; }
	private:
		vec min_, max_;
};

#endif

