CC=g++-4.7
CFLAGS=-std=c++0x -O0 -Wall -ggdb -DDEBUG
SRC=$(wildcard *.cc)
HEAD=$(wildcard *.h)
OBJ=$(SRC:.cc=.o)
TEMPLATES=$(wildcard *.hpp)
TARGET=poly

%.o: %.cc $(HEAD) $(TEMPLATES)
	$(CC) -c $(CFLAGS) $< -o $@

all: $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

clean:
	-rm $(OBJ) $(TARGET)

