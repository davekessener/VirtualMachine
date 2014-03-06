#!/bin/bash
make
g++-4.7 -c -std=c++0x -O0 -Wl,--no-as-needed `sdl2-config --cflags` -ggdb -DDEBUG -I.. test.cpp -o test.o
g++-4.7 test.o libeditor.a -o test `sdl2-config --libs` -lpthread -lSDL2_image
rm -f *.log
./test

