.PHONY: all clean

all:
	./make.sh

clean:
	./clean.sh

convert: convert.cpp
	g++-4.7 -std=gnu++11 -O3 -Wall -I/home/dave/include convert.cpp -o convert -L/home/dave/include/lib -ldav_Logger -ldav_gzstream -lz

