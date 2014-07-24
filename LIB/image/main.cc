#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>
#include <cstdint>
#include <cstdio>
#include "image.hpp"
#include "io.hpp"
#include "colors.hpp"
#include "noise.h"

#define MXT_PIXC 512

template<typename C>
void writeBMP(const std::string&, const lib::img::image<C>&);

int main(int argc, char *argv[])
{
	std::vector<std::string> args(argv, argv + argc);

	double lacunarity(1.9), gain(0.65);

	if(args.size() >= 3)
	{
		lacunarity = lib::aux::lexical_cast<double>(args.at(1).c_str());
		gain = lib::aux::lexical_cast<double>(args.at(2).c_str());
	}

	using namespace lib;
	using namespace img;
	typedef RGB8 COLOR;

	image<COLOR> standard(MXT_PIXC, MXT_PIXC), 
				 wood(MXT_PIXC, MXT_PIXC), 
				 fractal(MXT_PIXC, MXT_PIXC),
				 brownian(MXT_PIXC, MXT_PIXC),
				 terrain(128, 128),
				 curve(1024, 128);
	aux::Noise noise(0x0123456789abcdef);

	COLOR red(0xff), green(0xff00), blue(0xff0000);
	COLOR black(0x0), white(0xffffff);
	COLOR brown(0x17527f), grey(0x808080), sand(0x17a0d4), forest(0x174125);

	for(size_t y = 0, h = MXT_PIXC ; y < h ; ++y)
	{
		for(size_t x = 0, w = MXT_PIXC ; x < w ; ++x)
		{
			double dx = x / static_cast<double>(w - 1),
				   dy = y / static_cast<double>(h - 1);

			dx = 2.0 * dx - 1.0;
			dy = 2.0 * dy - 1.0;

#define NOISE(a,b,c) ((noise.noise3d(a,b,c)+1.0)*0.5)
			double r0(NOISE(dx, dy, 0.5));
			double r1(NOISE(dx * 8.0, dy * 8.0, 1.5));
			double r2(NOISE(dx * 32.0, dy * 32.0, 2.5));
			double r3(NOISE(dx * 128.0, dy * 128.0, 3.5));
			double r4(NOISE(dx * 1024.0, dy * 1024.0, 4.5));
#undef NOISE

			double r(r1);
			double r_wood(20.0 * r0 - static_cast<unsigned int>(20.0 * r0));
			double r_frac((r0 * 8 + r1 * 4 + r2 + 2 + r3 + r4) * 0.0625);
			double r_brown(noise.noise_fractal_brownian_motion(8, dx * 1.0, dy * 1.0, 1.5, lacunarity, gain));

			standard[x][y] = r * white;//r >= 0.5 ? white : black;
			wood[x][y] = r_wood * white;
			fractal[x][y] = r_frac * white;
			brownian[x][y] = r_brown * white;//r_brown >= 0.5 ? white : black;
		}
	}

	for(size_t y = 0, h = terrain.height() ; y < h ; ++y)
	{
		for(size_t x = 0, w = terrain.width() ; x < w ; ++x)
		{
			double 	dx = 2.0 * (x / (double)(w - 1)) - 1.0, 
					dy = 2.0 * (y / (double)(h - 1)) - 1.0;
			double 	tx = 1.0 - dx * dx * (dx < 0.0 ? -dx : dx),
					ty = 1.0 - dy * dy * (dy < 0.0 ? -dy : dy);
			const double s = 6.5;

			double r = 0.5 + 0.5 * noise.noise3d(s * dx, s * dy, 0.5);
			r *= tx * ty;
			r = 2.0 * r - 1.0;
//			r = r * r * r;
//			r = r < 0.0 ? -(r * r) : r * r;
//			r = r < 0.0 ? r * (1.0 - (1.0 + r) * (1.0 + r))
//						: r * (1.0 - (1.0 - r) * (1.0 - r));
			r = r < 0.0 ? r * (1.0 - (1.0 + r) * (1.0 + r) * (1.0 + r) * (1.0 + r))
						: r * (1.0 - (1.0 - r) * (1.0 - r) * (1.0 - r) * (1.0 - r));
			r = 0.5 + 0.5 * r;
//			r = 1.0 - (1.0 - r) * (1.0 - r);

			terrain[x][y] = r < 0.4 ? blue :
							r < 0.44 ? sand :
							r < 0.45 ? brown :
							r < 0.55 ? green :
							r < 0.7  ? forest :
							r < 0.85 ? grey :
							white;
		}
	}

	curve.fill(white);

	for(size_t x = 0, w = curve.width(), h = curve.height() ; x < w ; ++x)
	{
		double dx = x / static_cast<double>(w - 1);
		double r = noise.noise3d(dx * 25.0, 0.5, 0.5);
		size_t y = static_cast<int>((1.0 + 0.95 * r) * h * 0.5);
		curve[x][y] = black;
	}

#define WRITE(bmp) writeBMP("tx_" #bmp ".bmp", bmp)
	WRITE(standard);
	WRITE(wood);
	WRITE(fractal);
	WRITE(curve);
	WRITE(terrain);
	WRITE(brownian);
#undef WRITE
	
	return 0;
}

template<typename C>
void writeBMP(const std::string& fn, const lib::img::image<C>& img)
{
	std::ofstream out(fn.c_str(), std::ios::out | std::ios::binary);

	assert(out.is_open());

	lib::img::dib_BITMAPINFOHEADER_t dib;
	dib.width = img.width();
	dib.height = img.height();
	dib.resolution.dx = dib.resolution.dy = lib::img::BMP_RES;
	dib.compression = static_cast<std::uint32_t>(lib::img::Compression::RGB);
	dib.size = 0;

	out << lib::img::create_bmp(dib, img);

	out.close();
}

