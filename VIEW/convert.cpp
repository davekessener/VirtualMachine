#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cassert>
#include <gz/gzstream>
#include <aux>
#include "inc.h"
#include "bmp_header.h"
#include "dav_header.h"
#include "encrypt_stream.hpp"

std::string convert(const std::string&);

int main(int argc, char *argv[])
{
	assert(argc>=2&&argc<=3);

	if(argc == 2)
	{
		convert(argv[1]);
	}
	else if(argc == 3)
	{
		assert(argv[1] == std::string("-v"));
		std::cout << convert(argv[2]);
	}

	return 0;
}

std::string convert(const std::string& fn)
{
	using namespace lib::bmp;

	bmp_header_t bmp;
	dib_BITMAPINFOHEADER_t dib;

	std::ifstream in(fn);

	assert(in.is_open());

	in.read(reinterpret_cast<char *>(&bmp), sizeof(bmp));
	int l = bmp.offset - sizeof(bmp);
#define min(a,b) (((a)<(b))?(a):(b))
	in.read(reinterpret_cast<char *>(&dib), min(l, (int)sizeof(dib)));
#undef min
	l -= sizeof(dib);

	assert(dib.bpp==24);

	while(l-- > 0)
	{
		in.get();
	}

	std::vector<char> buf(bmp.filesize - bmp.offset);
	in.read(&*buf.begin(), buf.size());
	in.close();

	dav::imgheader_t dav;
	dav.id = dav::DAV_MAGIC;
	dav.width = dib.width;
	dav.height = dib.height;
	dav.filesize = sizeof(dav) + dav.width * dav.height * 3;

	int row = buf.size() / dib.height;
	const char *p = reinterpret_cast<const char *>(&*buf.cbegin());

	std::string hash(lib::aux::to_hex(dav::hash(fn)) + ".gz");
	gzip::ogzstream out_raw(hash.c_str());
	encrypt_stream<gzip::ogzstream> out(out_raw);
	
	out.write(reinterpret_cast<const char *>(&dav), sizeof(dav));
	for(int h = dib.height ; h ; --h)
	{
		for(int w = 0 ; w < dib.width ; ++w)
		{
			const char *t = p + (h - 1) * row + w * 3;
			out.put(t[2]);
			out.put(t[1]);
			out.put(t[0]);
		}
	}
	out.close();

	return hash;
}

