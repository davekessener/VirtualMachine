CC=gcc
SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)
HEAD=$(wildcard *.h)
CFLAGS=-ggdb -O0 -Wall
LIBS=-pthread -lrt
TARGET=lab2

.PHONY: all clean

%.o: %.c $(HEAD)
	$(CC) -c $(CFLAGS) $< -o $@

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LIBS)

clean:
	rm -f *.o $(TARGET)

