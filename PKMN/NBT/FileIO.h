#ifndef NBTFILE_IO_H
#define NBTFILE_IO_H

#include <string>
#include "Reader.h"
#include "Writer.h"
#include "NBTMain.h"

namespace nbt
{
	TAG_Compound::ptr_t readFile(const std::string&);
	void writeFile(const std::string&, TAG_Compound::ptr_t, bool = false);
}

#endif

