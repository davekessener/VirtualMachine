CC=g++-4.7
SDLCFLAGS=$(shell sdl2-config --cflags)
SDLLIBS=$(shell sdl2-config --libs)
CFLAGS=-Wall -Wno-sign-compare -std=c++0x -O0 -Wl,--no-as-needed -ggdb -DDEBUG $(SDLCFLAGS)
LIBS=$(SDLLIBS) -lSDL2_image -lSDL2_ttf -lpthread -lz
SRC=$(wildcard *.cc)
OBJ=$(SRC:.cc=.o)
HEADERS=$(wildcard *.h)
TEMPLATES=$(wildcard *.hpp)
SUBS=NBT.a GZ.a
TARGET=simulator

%.o: %.cc $(HEADERS) $(TEMPLATES)
	$(CC) -c $(CFLAGS) -I. $< -o $@

%.a:
	$(MAKE) -C $(@:.a=)/
	cp $(@:.a=)/*.a .

%.clean:
	-$(MAKE) -C $(@:.clean=)/ clean

$(TARGET): logs $(SUBS) $(OBJ)
	$(CC) $(OBJ) -Wl,--start-group $(shell ls *.a) -Wl,--end-group -o $(TARGET) $(LIBS)

.PHONY: logs clean

logs:
	-mv resource/*.log resource/logs/
	-rm -f *.a

clean: logs $(SUBS:.a=.clean)
	-rm -f *.o $(TARGET)

