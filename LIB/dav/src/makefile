CC=g++-4.7
SDLFLAGS=$(shell sdl2-config --cflags)
CFLAGS=-std=c++0x -O3 -Wall -Wl,--no-as-needed $(SDLFLAGS)
INCDIRS=-I.. -I/home/dave/include
SRC=$(wildcard *.cc)
OBJ=$(SRC:.cc=.o)

%.o: %.cc
	$(CC) -c $(CFLAGS) $(INCDIRS) $< -o $@

all: $(OBJ)
	for file in *.o; do ar rcs libdav_$${file%.o}.a $$file; ranlib libdav_$${file%.o}.a; done
	mv *.a ../../lib/

clean:
	rm -f *.o

