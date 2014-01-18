CC=g++
CFLAGS=-std=c++0x -ggdb -DDEBUG
SRC=$(wildcard *.cc)
OBJ=$(SRC:.cc=.o)
TEMPLATES=$(wildcard *.cpp)
TARGET=test

%.o: %.cc $(TEMPLATES)
	$(CC) -c $(CFLAGS) $< -o $@

all: clean $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

clean:
	-rm $(OBJ) $(TARGET)

