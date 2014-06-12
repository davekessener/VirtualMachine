CC=g++
CFLAGS=-std=c++0x
DEBUG=-ggdb -DDEBUG
SRC=$(wildcard *.cc)
OBJ=$(SRC:.cc=.o)
TARGET=libgz.a

%.o: %.cc %.h
	$(CC) -c $(CFLAGS) $(DEBUG) $< -o $@

$(TARGET): $(OBJ)
	ar rcs $(TARGET) $(OBJ)
	ranlib $(TARGET)

clean:
	-rm *.o $(TARGET)

