CC=g++-4.7
SDLCFLAGS=`sdl2-config --cflags`
SDLLIBS=`sdl2-config --libs`
CFLAGS=-std=c++0x -Wl,--no-as-needed -Wno-write-strings -ggdb -DDEBUG $(SDLCFLAGS)
LIBS=$(SDLLIBS) -lpthread
SRC=$(wildcard *.cc)
OBJ=$(SRC:.cc=.o)
HEADERS=$(wildcard *.h)
TARGET=pokemon

%.o: %.cc $(HEADERS)
	$(CC) -c $(CFLAGS) -I. $< -o $@

$(TARGET): libnbt.a libgz.a $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LIBS)

.PHONY: libnbt.a libgz.a clean

libgz.a:
	$(MAKE) -C GZ/
	cp GZ/libgz.a .

libnbt.a:
	$(MAKE) -C NBT/
	cp NBT/libnbt.a .

clean:
	$(MAKE) -C NBT/ clean
	$(MAKE) -C GZ/ clean
	-rm *.a *.o $(TARGET)

