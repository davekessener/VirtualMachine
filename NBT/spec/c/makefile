CC=gcc
CFLAGS=-O0 -Wall -Wl,--no-as-needed
DEBUG=-ggdb -gdwarf-3 -DNDEBUG
SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)
TARGET=libnbt.a

%.o: %.c %.h
	$(CC) -c $(CFLAGS) $(DEBUG) -I.. $< -o $@

$(TARGET): $(OBJ)
	ar rcs $(TARGET) $(OBJ)
	ranlib $(TARGET)
	cp $(TARGET) ~/include/lib/

run: $(OBJ)
	$(CC) $(OBJ) -o test

.PHONY: clean

clean:
	rm -f *.o $(TARGET)

