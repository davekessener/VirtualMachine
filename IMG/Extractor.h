#ifndef EXTRACTOR_H
#define EXTARCTOR_H

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include "IMG.h"
#include "Palette.h"
//#include "Image.h"
#include "basic_image.h"
#include "palette_image.h"
#include "Map.h"
#include "Vector.h"
#include "GBA.h"

class Extractor
{
	public:
		static int run(int, char **);
	private:
		static void write_to_disk(IMG::MAP&);
};

#endif

