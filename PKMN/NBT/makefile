CC=g++-4.7
CFLAGS=-std=c++0x -O0 -Wl,--no-as-needed
DEBUG=-ggdb -DDEBUG
SRC=$(wildcard *.cc)
OBJ=$(SRC:.cc=.o)
TARGET=libnbt.a

%.o: %.cc %.h
	$(CC) -c $(CFLAGS) $(DEBUG) -I.. $< -o $@

$(TARGET): $(OBJ)
	ar rcs $(TARGET) $(OBJ)
	ranlib $(TARGET)

.PHONY: clean

clean:
	-rm *.o $(TARGET)

