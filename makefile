CC=g++-4.7
SDLCFLAGS=$(shell sdl2-config --cflags)
SDLLIBS=$(shell sdl2-config --libs)
PNGCFLAGS=$(shell libpng16-config --cflags)
PNGLIBS=$(shell libpng16-config --ldflags)
CFLAGS=-std=c++0x -O0 -Wall -Wl,--no-as-needed -ggdb -DDEBUG $(SDLCFLAGS) $(PNGCFLAGS)
GLLIBS=-lGL -lglut
LIBS=$(SDLLIBS) $(PNGLIBS) $(GLLIBS) -lSDL2_image -lSDL2_ttf -lpthread -lz
SRC=$(wildcard *.cc)
OBJ=$(SRC:.cc=.o)
HEADERS=$(wildcard *.h)
TEMPLATES=$(wildcard *.hpp)
SUBS=
TARGET=

%.o: %.cc $(HEADERS) $(TEMPLATES)
	$(CC) -c $(CFLAGS) -I. -I/home/dave/include $< -o $@

%.a:
	$(MAKE) -C $(@:.a=)/
	cp $(@:.a=)/*.a .

%.clean:
	-$(MAKE) -C $(@:.clean=)/ clean

$(TARGET): logs $(SUBS) $(OBJ)
	$(CC) $(OBJ) -Wl,--start-group $(shell find -maxdepth 1 -name "*.a") -Wl,--end-group -o $(TARGET) $(LIBS)

.PHONY: logs clean

logs:
	for file in *.log; do if [ -f "$$file" ]; then mv "$$file" logs/; fi; done
	rm -f *.a

clean: logs $(SUBS:.a=.clean)
	-rm -f *.o $(TARGET)

