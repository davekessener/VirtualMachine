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
#include "math.hpp"
#include "aux.hpp"

#include "misc.h"

#define MXT_PIXC 512

template<typename C>
	void writeBMP(const std::string&, const lib::img::image<C>&);
int test_read(const std::vector<std::string>&);
int test_write(const std::vector<std::string>&);
int test_gl(const std::vector<std::string>&);
int test_math(const std::vector<std::string>&);

typedef std::pair<const unsigned char *, int> param_t;

param_t *update_gl();
param_t *show_image(const char *);
inline param_t *show_image_gl( ) { return show_image(nullptr); }
extern int gl_main(param_t *(*)(void));

lib::img::image<lib::img::RGB8> *load_image(const std::string&);

//std::pair<param_t, bool> gParam;
//param_t *show_image_gl(void)
//{
//	if(gParam.second)
//	{
//		gParam.second = false;
//		return &gParam.first;
//	}
//	else
//	{
//		return nullptr;
//	}
//}

int main(int argc, char *argv[])
{
	std::vector<std::string> args(argv, argv + argc);

//	lib::bmp_data *d = lib::load_bmp_from_file(args.at(1).c_str());
//	gParam.first.first = d->data;
//	gParam.first.second = d->width | (d->height << 16);
//	gParam.second = true;
//
//	int r = gl_main(&show_image_gl);
//
//	free(d);
//
//	return r;

//	show_image(args.at(1).c_str());

	return test_write(args);//gl_main(&update_gl);
}

param_t *show_image(const char *fn)
{
	using namespace lib::img;

	typedef std::shared_ptr<param_t> param_ptr;
	typedef std::shared_ptr<image<RGB8>> img_ptr;
	typedef std::pair<param_ptr, img_ptr> data_t;
	typedef std::shared_ptr<data_t> data_ptr;

	static data_ptr p;
	static bool loaded(false);

	if(fn)
	{
//		p.reset(new data_t);
		p = data_ptr(new data_t);
//		p->first.reset(new param_t);
		p->first = param_ptr(new param_t);
//		p->second.reset(load_image(fn));
		p->second = img_ptr(load_image(fn));
		p->first->first = reinterpret_cast<unsigned char *>(p->second->raw());
		p->first->second = (p->second->width() | (p->second->height() << 16));

		loaded = true;
	}
	else if(loaded)
	{
		if(static_cast<bool>(p))
		{
			loaded = false;

			return &*(p->first);
		}
	}
	else
	{
		loaded = true;
		p.reset();
	}

	return nullptr;
}

int test_math(const std::vector<std::string>& args)
{
	using namespace lib::aux;

	matrix<double, 2, 3> m1;
	matrix<double, 3, 4> m2;

//	m1[0][0] = 2;
//	m1[0][1] = 3;
//	
//	m2[0][0] = 3;
//	m2[1][0] = 1;
//	m2[0][1] = 4;
//	m2[1][1] = 1;
//	m2[0][2] = 5;
//	m2[1][2] = 1;

	m1[0][0] = 1;
	m1[0][1] = 2;
	m1[0][2] = 3;
	m1[1][0] = 4;
	m1[1][1] = 5;
	m1[1][2] = 6;

	m2[0][0] =  7;
	m2[0][1] =  8;
	m2[0][2] =  9;
	m2[0][3] = 10;
	m2[1][0] = 11;
	m2[1][1] = 12;
	m2[1][2] = 13;
	m2[1][3] = 14;
	m2[2][0] = 15;
	m2[2][1] = 16;
	m2[2][2] = 17;
	m2[2][3] = 18;

	auto m(m1 * m2);

	std::cout << m1 << " * " << m2 << std::endl
			  << "== " << m << std::endl;

	return 0;
}

template<typename T>
void register_cleanup(T *pT)
{
	static std::shared_ptr<T> p;
	p.reset(pT);
//	static std::vector<std::shared_ptr<T>> v;
//	v.push_back(std::shared_ptr<T>(pT));
}

template<typename COLOR>
void generate_terrain(const lib::aux::Noise& noise, lib::img::image<COLOR>& terrain, double z)
{
#define AD(v) (((v)<<(sizeof(COLOR)*8-24))|((1<<(sizeof(COLOR)*8-24))-1))
	COLOR red(AD(0xff)), green(AD(0xff00)), blue(AD(0xff0000));
	COLOR brown(AD(0x17527f)), grey(AD(0x808080)), sand(AD(0x17a0d4)), forest(AD(0x174125));
	COLOR black(0), white(AD(0xffffff));

	static auto trans = [](const COLOR& c) { return COLOR(AD((c.r() << 16) | (c.g() << 8) | c.b())); };
#undef AD

	for(size_t y = 0, h = terrain.height() ; y < h ; ++y)
	{
		for(size_t x = 0, w = terrain.width() ; x < w ; ++x)
		{
			double 	dx = 2.0 * (x / (double)(w - 1)) - 1.0, 
					dy = 2.0 * (y / (double)(h - 1)) - 1.0;
			double 	tx = 1.0 - dx * dx * (dx < 0.0 ? -dx : dx),
					ty = 1.0 - dy * dy * (dy < 0.0 ? -dy : dy);
			const double s = 6.5;

			double r = 0.5 + 0.5 * noise.noise3d(s * dx, s * dy, z);
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

			terrain[x][y] = (r < 0.4 ? blue :
							      r < 0.44 ? sand :
							      r < 0.45 ? brown :
							      r < 0.55 ? green :
							      r < 0.7  ? forest :
							      r < 0.85 ? grey :
							      white);
		}
	}
}

param_t *update_gl(void)
{
	using namespace lib;
	using namespace img;

	static const RGB8 white(0xffffff);
	static aux::Noise noise(0x123456789abcdef);
	static double z = 0.5;

//	std::vector<RGB8> *buf = new std::vector<RGB8>(MXT_PIXC * MXT_PIXC);
	image<RGB8> *img = new image<RGB8>(MXT_PIXC, MXT_PIXC);

	register_cleanup(img);

	z += 0.0025;

	generate_terrain(noise, *img, z);

//	for(int y = 0 ; y < MXT_PIXC ; ++y)
//	{
//		for(int x = 0 ; x < MXT_PIXC ; ++x)
//		{
//			double dx = 1.0 - 2.0 * (x / (double)(MXT_PIXC - 1)),
//				   dy = 1.0 - 2.0 * (y / (double)(MXT_PIXC - 1));
//
//			double r = 0.5 + 0.5 * noise.noise3d(dx, dy, z);
//			r = (r * 20.0) - static_cast<int>(r * 20.0);
//
//			buf->at(x + y * MXT_PIXC) = r * white;
//		}
//	}

	param_t *p = new param_t;
	p->first = reinterpret_cast<unsigned char *>(img->raw());
	p->second = MXT_PIXC;

	register_cleanup(p);

	return p;
}

lib::img::image<lib::img::RGB8> *load_image(const std::string& fn)
{
	using namespace lib::img;

	std::ifstream in(fn.c_str());

	assert(in.is_open());

	bmp_header_t bmp;
	dib_BITMAPINFOHEADER_t dib;
	in.read(reinterpret_cast<char *>(&bmp), sizeof(bmp));
	in.read(reinterpret_cast<char *>(&dib), sizeof(dib));
//	std::vector<RGB8> buf(dib.width * dib.height);
	image<RGB8> *p(new image<RGB8>(dib.width, dib.height));
	in.read(reinterpret_cast<char *>(p->raw()), dib.width * dib.height * sizeof(RGB8));

	in.close();

//	std::transform(p->cbegin(), p->cend(), p->begin(), [](const RGB8& c)
//		{
//			return RGB8((c.r() << 16) | (c.g() << 8) | c.b());
//		});
	
	if(false)
	{
		std::vector<RGB8> v(dib.width);
		auto i = p->begin();
		for(int h = dib.height ; h > dib.height / 2 ; --h)
		{
			std::vector<RGB8>::iterator j = p->begin() + (h - 1) * dib.width;
			lib::aux::copy_n(v.begin(), j, dib.width);
			lib::aux::copy_n(j, i, dib.width);
			lib::aux::copy(i, v.cbegin(), v.cend());
		}
//		std::vector<RGB8> v(buf.size());
//		auto i = v.begin();
//		for(int h = dib.height ; h ; --h)
//		{
//			aux::copy_n(i, buf.begin() + (h - 1) * dib.width, dib.width);
//		}
//		buf = v;
	}

	return p;
}

int test_gl(const std::vector<std::string>& args)
{
	using namespace lib;
	using namespace img;


	return 0;//gl_main(reinterpret_cast<std::uint8_t *>(&*buf.begin()), dib.width, dib.height);
}

int test_read(const std::vector<std::string>& args)
{
	if(args.size() < 2) return 1;

	using namespace lib;
	using namespace img;

	std::string fn("tx_" + args.at(1) + ".bmp");

	std::ifstream in(fn.c_str());
	bmp_reader bmp(read_bmp(in));
	in.close();

	image<RGB8> img(bmp.dib_h.width, bmp.dib_h.height);
	std::copy_n(bmp.iter, img.width() * img.height() * sizeof(RGB8), reinterpret_cast<std::uint8_t *>(img.raw()));

	writeBMP("tx_inv.bmp", img);

	return 0;
}

int test_write(const std::vector<std::string>& args)
{
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
				 terrain(MXT_PIXC, MXT_PIXC),
				 curve(1024, 128);
	aux::Noise noise(0x0123456789abcdef);

	COLOR black(0xff), white(0xffffffff);

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

	curve.fill(white);

	for(size_t x = 0, w = curve.width(), h = curve.height() ; x < w ; ++x)
	{
		double dx = x / static_cast<double>(w - 1);
		double r = noise.noise3d(dx * 25.0, 0.5, 0.5);
		size_t y = static_cast<int>((1.0 + 0.95 * r) * h * 0.5);
		curve[x][y] = black;
	}

	generate_terrain(noise, terrain, 1.5);

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

