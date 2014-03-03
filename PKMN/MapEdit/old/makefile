CC=g++-4.7
CFLAGS=-std=c++0x -Wl,--no-as-needed $(shell sdl2-config --cflags)
DEBUG=-ggdb -DDEBUG
SRC=$(wildcard *.cc)
OBJ=$(SRC:.cc=.o)
TARGET=libeditor.a

%.o: %.cc %.h
	$(CC) -c $(CFLAGS) $(DEBUG) -I.. $< -o $@

$(TARGET): $(OBJ)
	ar rcs $(TARGET) $(OBJ)
	ranlib $(TARGET)

.PHONY: clean

clean:
	-rm *.o $(TARGET)

