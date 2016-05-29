CC=g++
CFLAGS=-Wall -std=c++03 -ggdb
SRC=$(wildcard *.cc)
OBJ=$(SRC:.cc=.o)
HEAD=$(wildcard *.h)
TARGET=hsm

.PHONY: all clean

%.o: %.cc $(HEAD)
	$(CC) -c $(CFLAGS) $< -o $@

all: $(TARGET)

$(TARGET): $(OBJ) $(HEAD)
	$(CC) $(OBJ) -o $(TARGET)

clean:
	rm -f *.o $(TARGET)

