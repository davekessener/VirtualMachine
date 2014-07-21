CC=g++-4.7
ALIBS=
SDLCFLAGS=$(shell sdl2-config --cflags)
SDLLIBS=$(shell sdl2-config --libs)
CFLAGS=-std=c++0x -O0 -Wall -Wl,--no-as-needed -ggdb -DDEBUG $(SDLCFLAGS)
LIBS=$(SDLLIBS) -lSDL2_image -lSDL2_ttf -lpthread -lz -lGL -lglut $(ALIBS)
SRC=$(wildcard *.cc)
OBJ=$(SRC:.cc=.o)
HEADERS=$(wildcard *.h)
TEMPLATES=$(wildcard *.hpp)
SUBS=
TARGET=gl

%.o: %.cc $(HEADERS) $(TEMPLATES)
	$(CC) -c $(CFLAGS) -I. $< -o $@

%.a:
	$(MAKE) -C $(@:.a=)/
	@cp $(@:.a=)/*.a .

%.clean:
	-$(MAKE) -C $(@:.clean=)/ clean

$(TARGET): logs $(SUBS) $(OBJ)
	$(CC) $(OBJ) -Wl,--start-group $(shell find -maxdepth 1 -name "*.a") -Wl,--end-group -o $(TARGET) $(LIBS)

.PHONY: logs clean

logs:
	@for file in *.log; do if [ -f "$$file" ]; then mv "$$file" logs/; fi; done
	@rm -f *.a

clean: logs $(SUBS:.a=.clean)
	@-rm -f *.o $(TARGET)

