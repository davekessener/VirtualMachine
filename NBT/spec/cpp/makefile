CC=g++
CFLAGS=-std=gnu++11 -O3 -Wl,--no-as-needed
DEBUG=-ggdb -DNDEBUG
SRC=$(wildcard *.cc)
OBJ=$(SRC:.cc=.o)
TARGET=libdav_NBT.a

%.o: %.cc %.h
	$(CC) -c $(CFLAGS) $(DEBUG) -I.. -I/home/dave/include $< -o $@

$(TARGET): $(OBJ)
	ar rcs $(TARGET) $(OBJ)
	ranlib $(TARGET)
	cp $(TARGET) ~/include/lib/

.PHONY: clean

clean:
	rm -f *.o $(TARGET)

