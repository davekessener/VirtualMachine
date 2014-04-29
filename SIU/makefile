CC=g++-4.7
CFLAGS=-std=c++0x -O0 -Wl,--no-as-needed -ggdb -DDEBUG
LIBS=-lpthread -lz
SRC=$(wildcard *.cc)
OBJ=$(SRC:.cc=.o)
HEADERS=$(wildcard *.h)
TARGET=units

%.o: %.cc $(HEADERS)
	$(CC) -c $(CFLAGS) -I. $< -o $@

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LIBS)

.PHONY: clean

clean:
	-rm -f *.o $(TARGET)

