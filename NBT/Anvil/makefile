CC=gcc
CFLAGS=
LIBS=-lz
SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)
HEADERS=$(wildcard *.h)
TARGET=anvil

%.o: %.c $(HEADERS)
	$(CC) -c $(CFLAGS) $< -o $@

all: $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LIBS)

