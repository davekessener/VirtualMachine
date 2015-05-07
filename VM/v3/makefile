CC=g++
CFLAGS=-Wall -std=gnu++11 -O0 -ggdb -gdwarf-3
LIB=
SRC=$(wildcard *.cc)
OBJ=$(SRC:.cc=.o)
HEAD=$(wildcard *.{h,hpp})
TARGET=vm

.PHONY: all clean

%.o: %.cc $(HEAD)
	$(CC) -c $(CFLAGS) $< -o $@

all: $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LIB)

clean:
	rm -f $(OBJ) $(TARGET)

