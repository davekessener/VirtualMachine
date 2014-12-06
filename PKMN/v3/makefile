CC=g++
SDLCFLAGS=$(shell sdl2-config --cflags)
PNGCFLAGS=$(shell libpng16-config --cflags)
SDLLIBS=$(shell sdl2-config --libs)
PNGLIBS=$(shell libpng16-config --ldflags)
CFLAGS=-std=gnu++11 -O0 -ggdb -gdwarf-3 -Wall -I/home/dave/include $(SDLCFLAGS) $(PNGCFLAGS)
LIBS=-L/home/dave/include/lib -lpng -lGL $(SDLLIBS) $(PNGLIBS)
SRC=$(wildcard *.cc)
OBJ=$(SRC:.cc=.o)
DEPS=$(wildcard *.h) $(wildcard *.hpp)
TARGET=test

.PHONY: all clean

%.o: %.cc $(DEPS)
	$(CC) -c $(CFLAGS) $< -o $@

all: $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) -Wl,--start-group $(LIBS) -Wl,--end-group

clean:
	rm -f *.o $(TARGET)

