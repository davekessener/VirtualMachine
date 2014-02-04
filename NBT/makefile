CC=g++
CFLAGS=-std=c++0x -ggdb -DDEBUG
LIBS=-L./GZ -L./NBT -lgz -lnbt
SRC=$(wildcard *.cc)
OBJ=$(SRC:.cc=.o)
HEADERS=$(wildcard *.h)
TARGET=nbtedit

%.o: %.cc $(HEADERS)
	$(CC) -c $(CFLAGS) -I. $< -o $@

all: $(OBJ)
	$(MAKE) -C GZ/
	$(MAKE) -C NBT/
	$(CC) $(OBJ) -o $(TARGET) $(LIBS)

run: all
	export LD_LIBRARY_PATH="./NBT;./GZ" && ./$(TARGET)

clean:
	-$(MAKE) clean -C GZ/
	-$(MAKE) clean -C NBT/
	-rm $(OBJ) $(TARGET)

