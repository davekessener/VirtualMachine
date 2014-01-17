CC=g++
CFLAGS=-std=c++0x -ggdb -DDEBUG
SRC=$(wildcard *.cc)
OBJ=$(SRC:.cc=.o)
TEMPLATES=$(wildcard *.cpp)
TARGET=test

%.o: %.cc $(TEMPLATES)
	$(CC) -c $(CFLAGS) $< -o $@

all: $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

clean:
	-rm $(OBJ) $(TARGET)

