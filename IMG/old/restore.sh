#!/bin/bash
FILES="Extractor.cc Extractor.h GBA.cc GBA.h IMG.h Image.cc Image.h LZ77.cc LZ77.h Map.cpp Map.h Pair.h Palette.cc Palette.h Queue.cpp Queue.h Vector.cpp Vector.h _Map.cpp _Map.h _Pair.h basic_image.cpp basic_image.h flow_image.cpp flow_image.h main.cc pVector.h palette_image.cc palette_image.h"
for f in $FILES
do
	wget "https://raw.githubusercontent.com/davekessener/VirtualMachine/f0ba77bc95501be49904e8a507e09c26179ec6aa/IMG/$f"
done

