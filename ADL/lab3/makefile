CC=g++-4.8
CFLAGS=-Wall -O0 -std=gnu++11
LIBS=
SRC=$(wildcard *.cc)
HEAD=$(wildcard *.{h,hpp})
OBJ=$(SRC:.cc=.o)
TARGET=lab3

.PHONY: all clean

%.o: %.cc $(HEAD)
	$(CC) -c $(CFLAGS) $< -o $@

All: $(TARGET)

$(TARGET): $(OBJ) $(HEAD)
	$(CC) $(OBJ) -o $(TARGET) $(LIBS)

clean:
	rm -f *.o $(TARGET)

