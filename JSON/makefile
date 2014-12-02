CC=g++
CFLAGS=-std=gnu++11 -O3
LIBS=
SRC=$(wildcard *.cc)
OBJ=$(SRC:.cc=.o)
DEPS=$(wildcard *.{h,hpp})
TARGET=json

.PHONY: all clean lib

%.o: %.cc %.h $(DEPS)
	$(CC) -c $(CFLAGS) $< -o $@

all: $(OBJ)
	$(CC) -c $(CFLAGS) main.cpp -o main.o
	$(CC) main.o $(OBJ) -o $(TARGET) $(LIBS)

clean:
	rm -f *.o $(TARGET) $(TARGET).a

lib: $(OBJ)
	ar rcs $(TARGET).a $(OBJ)
	ranlib $(TARGET).a

