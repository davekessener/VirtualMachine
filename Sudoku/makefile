CC=g++
CFLAGS=-std=c++0x -ggdb -DDEBUG
LIBS=-lncursesw
SRC=$(wildcard *.cc)
OBJ=$(SRC:.cc=.o)
HEADERS=$(wildcard *.h)
DEPS=
TARGET=sudoku

%.o: %.cc $(HEADERS)
	$(CC) -c $(CFLAGS) -I. $< -o $@

$(TARGET): $(OBJ) $(DEPS)
	$(CC) $(OBJ) -o $(TARGET) $(LIBS)

clean:
	-rm $(OBJ) $(TARGET)

