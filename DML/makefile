CC=g++-4.7
CFLAGS=-std=c++0x -ggdb -O0 -DDEBUG
SRC=$(wildcard *.cc)
OBJ=$(SRC:.cc=.o)
TEMPLATES=$(wildcard *.hpp)
TARGET=test

%.o: %.cc $(TEMPLATES)
	$(CC) -c $(CFLAGS) $< -o $@

all: $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

clean:
	-rm *.o $(TARGET)

