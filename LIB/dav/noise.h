#ifndef LIB_NOISE_H
#define LIB_NOISE_H

namespace lib
{
	namespace aux
	{
		class Noise
		{
			public:
				Noise(unsigned long = 0);
				double noise3d(double, double, double) const;
				double noise_fractal_brownian_motion(int, double, double, double, double = 1.9, double = 0.65) const;
				inline double noise3d_t(double x, double y, double z) const { return 0.5 + 0.5 * noise3d(x, y, z); }
			private:
				static inline double fade(double t)
				{
					return t * t * t * (t * (t * 6 - 15) + 10);
				}
				static inline double lerp(double t, double a, double b)
				{
					return a + t * (b - a);
				}
				static inline double grad3d(int h, double x, double y, double z)
				{
					h &= 0x0f;
					double u(h < 8 ? x : y), v(h < 4 ? y : (h == 12 || h == 14) ? x : z);
					return ((h & 1) ? -u : u) + ((h & 2) ? -v : v);
				}
			private:
				int p_[0x200];
				unsigned long seed_;
		};
	}
}

#endif

