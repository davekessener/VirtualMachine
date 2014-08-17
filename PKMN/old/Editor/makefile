CC=g++-4.7
INCDIR=-I$(shell echo $$HOME)/include
CFLAGS=-std=c++0x -O0 -Wl,--no-as-needed $(shell sdl2-config --cflags) $(INCDIR)
DEBUG=-ggdb -DDEBUG
SRC=$(wildcard *.cc)
OBJ=$(SRC:.cc=.o)
HEADER=$(wildcard *.h)
TARGET=libeditor.a

%.o: %.cc $(HEADER)
	$(CC) -c $(CFLAGS) $(DEBUG) -I.. $< -o $@

$(TARGET): $(OBJ)
	ar rcs $(TARGET) $(OBJ)
	ranlib $(TARGET)

.PHONY: clean

clean:
	-rm *.o $(TARGET)

