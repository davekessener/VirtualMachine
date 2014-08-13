CC=g++-4.7
SDLCFLAGS=$(shell sdl2-config --cflags)
PNGCFLAGS=$(shell libpng16-config --cflags)

PNGLIBS=$(shell libpng16-config --ldflags)
SDLLIBS=$(shell sdl2-config --libs)
GLLIBS=-lGL -lglut
BOOSTLIBS=-lboost_system -lboost_filesystem
DAVLIBS=-ldav_gz -ldav_gl -ldav_sdl

LIBDIRS=-L/home/dave/include/lib
INCDIRS=-I. -I/home/dave/include 

CFLAGS=-std=c++0x -O4 -Wall -Wl,--no-as-needed -ggdb $(SDLCFLAGS) $(PNGCFLAGS)
LIBS=$(DAVLIBS) $(SDLLIBS) $(PNGLIBS) $(GLLIBS) $(BOOSTLIBS) -lSDL2_image -lSDL2_ttf -lpthread -lz

SRC=$(wildcard *.cc)
OBJ=$(SRC:.cc=.o)
HEADERS=$(wildcard *.h)
TEMPLATES=$(wildcard *.hpp)
SUBS=


TARGET=vm

%.o: %.cc $(HEADERS) $(TEMPLATES)
	$(CC) -c $(CFLAGS) $(INCDIRS) $< -o $@

%.a:
	$(MAKE) -C $(@:.a=)/
	cp $(@:.a=)/*.a .

%.clean:
	-$(MAKE) -C $(@:.clean=)/ clean

all: makefile $(TARGET)

$(TARGET): logs $(SUBS) $(OBJ)
	$(CC) $(OBJ) -Wl,--start-group $(shell find -maxdepth 1 -name "*.a") -Wl,--end-group $(LIBDIRS) -o $(TARGET) $(LIBS)

.PHONY: logs clean

logs:
	for file in *.log; do if [ -f "$$file" ]; then mv "$$file" logs/; fi; done

clean: logs $(SUBS:.a=.clean)
	-rm -f *.o $(TARGET)

