CC=g++
CFLAGS=-std=gnu++11 -O0 -ggdb -Wall -I/home/dave/include
LIBS=-L/home/dave/include/lib -ldav_NBT
SRC=$(wildcard *.cc)
OBJ=$(SRC:.cc=.o)
DEPS=$(wildcard *.{h,hpp})
TARGET=json

.PHONY: clean all

%.o: %.cc $(DEPS)
	$(CC) -c $(CFLAGS) $< -o $@

all: $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LIBS)

clean:
	rm -f *.o $(TARGET)

