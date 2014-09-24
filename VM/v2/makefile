CC=gcc-4.7
CFLAGS=-Wall -O0 -ggdb
SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)
HEADERS=$(wildcard *.h)
TARGET=vm

%.o: %.c $(HEADERS)
	$(CC) -c $(CFLAGS) $< -o $@

.PHONY: clean

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET)

clean:
	rm -f *.o $(TARGET)

