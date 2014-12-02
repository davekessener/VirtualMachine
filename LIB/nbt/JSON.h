#ifndef NBT_JSON_H
#define NBT_JSON_H

#include <iosfwd>
#include "NBTMain.h"

namespace nbt
{
	namespace json
	{
		TAG_Compound_ptr_t read(std::istream&);
		void write(std::ostream&, TAG_Compound_ptr_t);
	}
}

#endif

