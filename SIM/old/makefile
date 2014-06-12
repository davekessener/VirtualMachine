CC=g++
CFLAGS=-ggdb -DDEBUG
SRC=$(wildcard *.cc)
OBJ=$(SRC:.cc=.o)

%.o: %.cc
	$(CC) -c $(CFLAGS) $< -o $@

all: $(OBJ)
	$(CC) $(OBJ) -o simulator

test: test.cpp
	$(CC) $(CFLAGS) test.cpp -o test

clean:
	-rm $(OBJ) simulator

