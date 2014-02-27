CC=g++
CFLAGS=-std=c++0x
DEBUG=-ggdb -DDEBUG
SRC=$(wildcard *.cc)
OBJ=$(SRC:.cc=.o)
TARGET=libnbt.a

%.o: %.cc %.h
	$(CC) -c $(CFLAGS) $(DEBUG) -I.. $< -o $@

$(TARGET): $(OBJ)
	ar rcs $(TARGET) $(OBJ)
	ranlib $(TARGET)

clean:
	-rm *.o $(TARGET)

