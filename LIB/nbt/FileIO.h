#ifndef NBT_FILEIO_H
#define NBT_FILEIO_H

#include <string>
#include "NBTMain.h"

namespace nbt
{
	TAG_Compound::ptr_t readFile(const std::string&);
	void writeFile(const std::string&, TAG_Compound::ptr_t, bool = false);
	std::ostream& operator<<(std::ostream&, const NBTBase&);
}

#endif

