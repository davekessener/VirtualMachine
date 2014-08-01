CC=g++-4.7
CFLAGS=-std=c++0x -Wall
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

