CC=g++
CFLAGS=-std=gnu++11 -Wall -Wno-unused-local-typedefs -ggdb -gdwarf-3 -I/home/dave/include
LIBS=-L/home/dave/include/lib
SRC=$(wildcard *.cc)
OBJ=$(SRC:.cc=.o)
HEAD=$(wildcard *.h)
TARGET=table

%.o: %.cc $(HEAD)
	$(CC) -c $(CFLAGS) $< -o $@

all: $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LIBS)

.PHONY: clean

clean:
	rm -f $(OBJ) $(TARGET)

