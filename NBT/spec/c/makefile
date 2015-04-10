CC=gcc
CFLAGS=-O3 -Wall -Wl,--no-as-needed
DEBUG=-ggdb -gdwarf-3 -DNDEBUG
SRC=$(wildcard *.c)
OBJ=$(SRC:.cc=.o)
TARGET=libnbt.a

%.o: %.c %.h
	$(CC) -c $(CFLAGS) $(DEBUG) -I.. $< -o $@

$(TARGET): $(OBJ)
	ar rcs $(TARGET) $(OBJ)
	ranlib $(TARGET)
	cp $(TARGET) ~/include/lib/

.PHONY: clean

clean:
	rm -f *.o $(TARGET)

