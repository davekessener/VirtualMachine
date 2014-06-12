CC=g++-4.7
SDLCFLAGS=$(shell sdl2-config --cflags)
CFLAGS=-std=c++0x -Wl,--no-as-needed -ggdb -DDEBUG $(SDLCFLAGS)
SRC=$(wildcard *.cc)
OBJ=$(SRC:.cc=.o)
HEADERS=$(wildcard *.h)
TARGET=libow.a

%.o: %.cc $(HEADERS)
	$(CC) -c $(CFLAGS) -I.. $< -o $@

$(TARGET): $(OBJ)
	ar rcs $(TARGET) $(OBJ)
	ranlib $(TARGET)

.PHONY: clean

clean:
	-rm *.o $(TARGET)

