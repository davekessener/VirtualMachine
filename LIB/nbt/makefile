CC=g++-4.7
CFLAGS=-std=c++0x -O3 -Wl,--no-as-needed
DEBUG=-ggdb -DDEBUG
SRC=$(wildcard *.cc)
OBJ=$(SRC:.cc=.o)
TARGET=libdav_nbt.a

%.o: %.cc %.h
	$(CC) -c $(CFLAGS) $(DEBUG) -I.. $< -o $@

$(TARGET): $(OBJ)
	ar rcs $(TARGET) $(OBJ)
	ranlib $(TARGET)
	cp $(TARGET) ~/include/lib/

.PHONY: clean

clean:
	rm -f *.o $(TARGET)

