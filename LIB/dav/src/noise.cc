#include <random>
#include <algorithm>
#include "noise.h"

namespace dav
{
	Noise::Noise(unsigned long seed) : seed_(seed)
	{
		std::iota(p_, p_ + 0x100, 0);
		std::shuffle(p_, p_ + 0x100, std::default_random_engine(seed));
		std::copy(p_, p_ + 0x100, p_ + 0x100);
	}
	
	double Noise::noise3d(double x, double y, double z) const
	{
		int X = static_cast<int>(x < 0.0 ? (x - 1.0) : x),
			Y = static_cast<int>(y < 0.0 ? (y - 1.0) : y),
			Z = static_cast<int>(z < 0.0 ? (z - 1.0) : z);
		
		x -= X; X &= 0xff; 
		y -= Y; Y &= 0xff; 
		z -= Z; Z &= 0xff; 
	
		double u = fade(x), 
			   v = fade(y), 
			   w = fade(z);
	
		int A = p_[X  ]+Y, AA = p_[A]+Z, AB = p_[A+1]+Z,
			B = p_[X+1]+Y, BA = p_[B]+Z, BB = p_[B+1]+Z;
	
		return lerp(w, lerp(v, lerp(u, grad3d(p_[AA  ], x,   y,   z  ),
									   grad3d(p_[BA  ], x-1, y,   z  )),
							   lerp(u, grad3d(p_[AB  ], x,   y-1, z  ),
							   		   grad3d(p_[BB  ], x-1, y-1, z  ))),
					   lerp(v, lerp(u, grad3d(p_[AA+1], x,   y,   z-1),
					   				   grad3d(p_[BA+1], x-1, y,   z-1)),
							   lerp(u, grad3d(p_[AB+1], x,   y-1, z-1),
							   		   grad3d(p_[BB+1], x-1, y-1, z-1))));
	}
	
	double Noise::noise_fractal_brownian_motion(int o, double x, double y, double z, double lacunarity, double gain) const
	{
	//	static const double lacunarity = 1.9;
	//	static const double gain = 0.65;
	
		double sum(0.0);
		double amplitude(1.0);
		double rect(0.0);
	
		while(o--)
		{
			sum += amplitude * noise3d_t(x, y, z);
			rect += amplitude;
			amplitude *= gain;
			x *= lacunarity;
			y *= lacunarity;
			z *= lacunarity;
		}
	
		return sum / rect;
	}
}

