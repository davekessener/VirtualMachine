CC=g++-4.7
CFLAGS=-std=c++0x -O0 -Wl,--no-as-needed $(shell sdl2-config --cflags)
DEBUG=-ggdb -DDEBUG
SRC=$(wildcard *.cc)
OBJ=$(SRC:.cc=.o)
TARGET=libeditor.a

%.o: %.cc %.h
	$(CC) -c $(CFLAGS) $(DEBUG) -I.. $< -o $@

$(TARGET): $(OBJ)
	ar rcs $(TARGET) $(OBJ)
	ranlib $(TARGET)

.PHONY: clean logs test

clean:
	-rm *.o $(TARGET)

logs:
	-rm -f *.log

test: logs $(TARGET)
	$(CC) -c $(CFLAGS) $(DEBUG) -I.. test.cpp -o test.o
	$(CC) test.o $(TARGET) -o test $(shell sdl2-config --libs) -lpthread -lSDL2_image -lSDL2_ttf
	./test

