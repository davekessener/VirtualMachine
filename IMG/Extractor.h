#ifndef EXTRACTOR_H
#define EXTARCTOR_H

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include "IMG.h"
#include "Palette.h"
//#include "Image.h"
#include "basic_image.h"
#include "palette_image.h"
#include "flow_image.h"
#include "Map.h"
#include "Vector.h"
#include "pVector.h"
#include "Queue.h"
#include "GBA.h"

class Extractor
{
	public:
		static int run(int, char **);
	private:
		static void write_to_disk(const std::string&, const IMG::rgba_image&);
		static std::string get_map_id(const IMG::MAP&);
		static IMG::rgba_image *generate_tilemap(IMG::MAP&);
};

#endif

