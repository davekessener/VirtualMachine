CC=g++
CFLAGS=-std=c++0x -ggdb -DDEBUG
LIBS=-lz
SRC=$(wildcard *.cc)
OBJ=$(SRC:.cc=.o)
TEMPLATES=$(wildcard *.cpp)
TARGET=nbtedit

%.o: %.cc $(TEMPLATES)
	$(CC) -c $(CFLAGS) $< -o $@

all: $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LIBS)

test:
	cd test
	make

clean:
	-rm $(OBJ) $(TARGET)

