CC=g++-4.7
CFLAGS=-std=c++0x -O0 -Wall -ggdb -DDEBUG
SRC=$(wildcard *.cc)
OBJ=$(SRC:.cc=.o)
TEMPLATES=$(wildcard *.hpp)
TARGET=imgex

%.o: %.cc $(TEMPLATES)
	$(CC) -c $(CFLAGS) $< -o $@

all: $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

test: test.c
	$(CC) $(CFLAGS) test.c -o test

clean:
	rm -f *.o $(TARGET)

