CC=gcc
CFLAGS=-Wall -ggdb -gdwarf-3 -O0
LIBS=
SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)
HEADER=$(wildcard *.h)
TARGET=lab2

%.o: %.c $(HEADER)
	$(CC) -c $(CFLAGS) $< -o $@

all: $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LIBS)

clean:
	rm -f *.o $(TARGET)

