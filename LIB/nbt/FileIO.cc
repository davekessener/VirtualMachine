#include "FileIO.h"
#include "Reader.h"
#include "Writer.h"

using namespace dav;

namespace nbt
{
	TAG_Compound::ptr_t readFile(const std::string& path)
	{
		TAG_Compound::ptr_t ptr;
		std::ifstream *in = new std::ifstream(path, std::ios::in | std::ios::binary);
		gzip::igzstream *gzin = NULL;

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
					gzin = new gzip::igzstream(path.c_str());
					ptr = Read(*gzin);
				}
				else
				{
					throw NBTException(
						std::string("ERR: Invalid first byte in file '") + path + "'!");
				}
			}
		}

		if(in) { in->close(); delete in; }
		if(gzin) { gzin->close(); delete gzin; }

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
			gzip::ogzstream out(path.c_str());

			assert(out.good());

			nbt->write(out);

			out.close();
		}
	}

	std::ostream& operator<<(std::ostream& os, const NBTBase& nbt)
	{
		nbt.write(os);

		return os;
	}
}

