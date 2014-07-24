#include <random>
#include <algorithm>
#include "noise.h"

namespace lib { namespace aux {

Noise::Noise(unsigned long seed) : seed_(seed)
{
	std::iota(p_, p_ + 0x100, 0);
	std::shuffle(p_, p_ + 0x100, std::default_random_engine(seed));
	std::copy(p_, p_ + 0x100, p_ + 0x100);
}

double Noise::noise3d(double x, double y, double z) const
{
	typedef unsigned int uint;

	uint X = static_cast<uint>(x),
		 Y = static_cast<uint>(y),
		 Z = static_cast<uint>(z);
	
	x -= X; X &= 0xff;
	y -= Y; Y &= 0xff;
	z -= Z; Z &= 0xff;

	double u = fade(x), v = fade(y), w = fade(z);

	uint A = p_[X  ]+Y, AA = p_[A]+Z, AB = p_[A+1]+Z,
		 B = p_[X+1]+Y, BA = p_[B]+Z, BB = p_[B+1]+Z;

	return lerp(w, lerp(v, lerp(u, grad(p_[AA  ], x,   y,   z  ),
								   grad(p_[BA  ], x-1, y,   z  )),
						   lerp(u, grad(p_[AB  ], x,   y-1, z  ),
						   		   grad(p_[BB  ], x-1, y-1, z  ))),
				   lerp(v, lerp(u, grad(p_[AA+1], x,   y,   z-1),
				   				   grad(p_[BA+1], x-1, y,   z-1)),
						   lerp(u, grad(p_[AB+1], x,   y-1, z-1),
						   		   grad(p_[BB+1], x-1, y-1, z-1))));
}

}}

