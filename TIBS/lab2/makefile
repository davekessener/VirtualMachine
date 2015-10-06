CC=gcc
SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)
HEAD=$(wildcard *.h)
CFLAGS=-ggdb -O0
LIBS=-pthread -lrt
TARGET=lab2

%.o: %.c $(HEAD)
	$(CC) -c $(CFLAGS) $< -o $@

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LIBS)

clean:
	rm -f *.o $(TARGET)

