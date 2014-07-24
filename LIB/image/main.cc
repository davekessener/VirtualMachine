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

int main(int argc, char *argv[])
{
	std::vector<std::string> args(argv, argv + argc);

	using namespace lib;
	using namespace img;
	typedef RGBA8 COLOR;

	image<COLOR> standard(MXT_PIXC, MXT_PIXC), 
				 wood(MXT_PIXC, MXT_PIXC), 
				 fractal(MXT_PIXC, MXT_PIXC);
	aux::Noise noise(0x0123456789abcdef);

	COLOR red(0xffff), green(0xff00ff), blue(0xff0000ff);
	COLOR black(0xff), white(0xffffffff);

	for(size_t y = 0, h = MXT_PIXC ; y < h ; ++y)
	{
		for(size_t x = 0, w = MXT_PIXC ; x < w ; ++x)
		{
			double dx = x / static_cast<double>(w - 1),
				   dy = y / static_cast<double>(h - 1);

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

			standard[x][y] = r * white;
			wood[x][y] = r_wood * white;
			fractal[x][y] = r_frac * white;
		}
	}

	dib_BITMAPINFOHEADER_t dib;
	dib.width = MXT_PIXC;
	dib.height = MXT_PIXC;
	dib.resolution.dx = dib.resolution.dy = BMP_RES;
	dib.compression = static_cast<std::uint32_t>(Compression::RGB);
	dib.size = 0;

	std::ofstream out;
	
	out.open("standard.bmp", std::ios::out | std::ios::binary);
	out << create_bmp(dib, standard);
	out.close();
	
	out.open("wood.bmp", std::ios::out | std::ios::binary);
	out << create_bmp(dib, wood);
	out.close();
	
	out.open("fractal.bmp", std::ios::out | std::ios::binary);
	out << create_bmp(dib, fractal);
	out.close();
	
	return 0;
}

