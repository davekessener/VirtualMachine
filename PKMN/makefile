CC=g++-4.7
SDLCFLAGS=`sdl2-config --cflags`
SDLLIBS=`sdl2-config --libs`
CFLAGS=-std=c++0x -Wl,--no-as-needed -ggdb -DDEBUG $(SDLCFLAGS)
LIBS=$(SDLLIBS) -lSDL2_image -lpthread -lz
SRC=$(wildcard *.cc)
OBJ=$(SRC:.cc=.o)
HEADERS=$(wildcard *.h)
TARGET=pokemon

%.o: %.cc $(HEADERS)
	$(CC) -c $(CFLAGS) -I. $< -o $@

$(TARGET): logs libnbt.a libgz.a $(OBJ)
	$(CC) $(OBJ) libnbt.a libgz.a -o $(TARGET) $(LIBS)

.PHONY: logs libnbt.a libgz.a clean

logs:
	rm -f *.log

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

