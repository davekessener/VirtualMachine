CC=g++
CFLAGS=-std=gnu++11 -Wall -O0 -ggdb -DDEBUG
LIBS=
SRC=$(wildcard *.cc)
OBJ=$(SRC:.cc=.o)
HEAD=$(wildcard *.{h,hpp})
TARGET=lab3

.PHONY: all clean

%.o: %.cc $(HEAD)
	$(CC) -c $(CFLAGS) $< -o $@

all: $(OBJ) $(HEAD)
	$(CC) $(OBJ) -o $(TARGET) $(LIBS)

clean:
	rm -f *.o $(TARGET)