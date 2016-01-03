CC=gcc
CFLAGS=-O2 -ggdb -Wall
LIBS=-lrt
SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)
HEADER=$(wildcard *.h)
TARGET=lab4u

.PHONY: all clean

%.o: %.c $(HEADER)
	$(CC) -c $(CFLAGS) $< -o $@

all: $(TARGET)

$(TARGET): $(OBJ) $(HEADER)
	$(CC) $(OBJ) -o $(TARGET) $(LIBS)

clean:
	rm -f *.o $(TARGET)

