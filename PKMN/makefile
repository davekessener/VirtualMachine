CC=g++-4.7
SDLCFLAGS=$(shell sdl2-config --cflags)
SDLLIBS=$(shell sdl2-config --libs)
CFLAGS=-std=c++0x -O0 -Wl,--no-as-needed -ggdb -DDEBUG $(SDLCFLAGS)
LIBS=$(SDLLIBS) -lSDL2_image -lpthread -lz
SRC=$(wildcard *.cc)
OBJ=$(SRC:.cc=.o)
HEADERS=$(wildcard *.h)
SUBS=NBT.a GZ.a MapEdit.a Misc.a Overworld.a
TARGET=pokemon

%.o: %.cc $(HEADERS)
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
	-rm -f *.log

clean: logs $(SUBS:.a=.clean)
	-rm -f *.a *.o $(TARGET)

