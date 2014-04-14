CC=g++
CFLAGS=-std=c++0x -ggdb -DDEBUG
SRC=$(wildcard *.cc)
OBJ=$(SRC:.cc=.o)
TEMPLATES=$(wildcard *.cpp)
TARGET=imgex

%.o: %.cc $(TEMPLATES)
	$(CC) -c $(CFLAGS) $< -o $@

all: $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

test: test.c
	$(CC) $(CFLAGS) test.c -o test

clean:
	-rm $(OBJ) $(TARGET)

