CC=g++-4.7
SDLCFLAGS=$(shell sdl2-config --cflags)
PNGCFLAGS=$(shell libpng16-config --cflags)
DAVCFLAGS=
SDLLIBS=$(shell sdl2-config --libs)
PNGLIBS=$(shell libpng16-config --ldflags)
GLLIBS=-lGL -lglut
GLIBS=-lSDL2_image -lSDL2_ttf -lpthread -lz
DAVLIBS=
DAVDIR=$(shell echo ~/include)
INCDIRS=-I$(DAVDIR)
LIBDIRS=-L$(DAVDIR)/lib
CFLAGS=-std=gnu++11 -O0 -Wall -Wl,--no-as-needed -ggdb -DDEBUG $(SDLCFLAGS) $(PNGCFLAGS) $(DAVCFLAGS) $(INCDIRS)
LIBS=$(LIBDIRS) -Wl,--start-group $(DAVLIBS) $(GLIBS) $(SDLLIBS) $(PNGLIBS) $(GLLIBS) -Wl,--end-group
SRC=$(wildcard *.cc)
OBJ=$(SRC:.cc=.o)
HEADERS=$(wildcard *.h)
TEMPLATES=$(wildcard *.hpp)
SUBS=
TARGET=

%.o: %.cc $(HEADERS) $(TEMPLATES)
	$(CC) -c $(CFLAGS) $< -o $@

%.a:
	$(MAKE) -C $(@:.a=)/
	cp $(@:.a=)/*.a .

%.clean:
	-$(MAKE) -C $(@:.clean=)/ clean

$(TARGET): logs $(SUBS) $(OBJ)
	$(CC) $(OBJ) -Wl,--start-group $(shell find -maxdepth 1 -name "*.a") -Wl,--end-group -o $(TARGET) $(LIBS)

.PHONY: logs clean cleanlibs

logs:
	for file in *.log; do if [ -f "$$file" ]; then mv "$$file" logs/; fi; done

cleanlibs:
	rm -f *.a

clean: logs $(SUBS:.a=.clean)
	rm -f *.o $(TARGET)

