CC=g++
CFLAGS=-std=c++0x -ggdb -DDEBUG
SRC=$(wildcard *.cc)
OBJ=$(SRC:.cc=.o)
TEMPLATES=$(wildcard *.ct)
TARGET=imgex

%.o: %.cc $(TEMPLATES)
	$(CC) -c $(CFLAGS) $< -o $@

all: $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

test: test.cpp
	$(CC) $(CFLAGS) test.cpp -o test

clean:
	-rm $(OBJ) $(TARGET)

