#include <fstream>
#include "FileIO.h"
#include "Reader.h"
#include "Writer.h"

#ifndef NBT_NO_GZ
using namespace dav;
#endif

namespace nbt
{
	TAG_Compound::ptr_t readFile(const std::string& path)
	{
		TAG_Compound::ptr_t ptr;
		std::ifstream *in = new std::ifstream(path, std::ios::in | std::ios::binary);
#ifndef NBT_NO_GZ
		gzip::igzstream *gzin = NULL;
#endif

		if(in->good())
		{
			char c; in->get(c);

			if(c == 0x0a)
			{
				in->unget();
				ptr = Read(*in);
			}
			else
			{
				in->close();
				delete in;
				in = NULL;
			
				if(c == 0x1f)
				{
#ifndef NBT_NO_GZ
					gzin = new gzip::igzstream(path.c_str());
					ptr = Read(*gzin);
#else
					throw NBTException(
						std::string("ERR: GZ compression not supported!"));
#endif
				}
				else
				{
					throw NBTException(
						std::string("ERR: Invalid first byte in file '") + path + "'!");
				}
			}
		}

		if(in) { in->close(); delete in; }
#ifndef NBT_NO_GZ
		if(gzin) { gzin->close(); delete gzin; }
#endif

		return ptr;
	}

	void writeFile(const std::string& path, TAG_Compound::ptr_t nbt, bool gz)
	{
		if(!gz)
		{
			std::ofstream out(path, std::ios::out | std::ios::binary);

			assert(out.good());

			nbt->write(out);

			out.close();
		}
		else
		{
#ifndef NBT_NO_GZ
			gzip::ogzstream out(path.c_str());

			assert(out.good());

			nbt->write(out);

			out.close();
#else
			throw NBTException(std::string("ERR: GZ compression not supported!"));
#endif
		}
	}

	std::ostream& operator<<(std::ostream& os, const NBTBase& nbt)
	{
		nbt.write(os);

		return os;
	}
}

