CC=g++-4.7
CFLAGS=-std=c++0x -Wl,--no-as-needed -ggdb -DDEBUG
LIBS=-lncursesw -lpthread
SRC=$(wildcard *.cc)
OBJ=$(SRC:.cc=.o)
TEMPLATES=$(wildcard *.cpp)
TARGET=test

%.o: %.cc $(TEMPLATES)
	$(CC) -c $(CFLAGS) $< -o $@

all: $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LIBS)

clean:
	-rm $(OBJ) $(TARGET)

