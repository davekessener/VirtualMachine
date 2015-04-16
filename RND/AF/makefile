CC=g++
CFLAGS=-std=gnu++14 -Wall -Wno-unused-local-typedefs -ggdb -gdwarf-3
LIBS=
SRC=$(wildcard *.cc)
OBJ=$(SRC:.cc=.o)
HEAD=$(wildcard *.{h,hpp})
TARGET=table

%.o: %.cc $(HEAD)
	$(CC) -c $(CFLAGS) $< -o $@

all: $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LIBS)

.PHONY: clean

clean:
	rm -f $(OBJ) $(TARGET)

