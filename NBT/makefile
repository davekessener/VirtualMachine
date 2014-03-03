CC=g++-4.7
CFLAGS=-std=c++0x -Wl,--no-as-needed -ggdb -DDEBUG
LIBS=-L./GZ -L./NBT -lgz -lnbt -lncursesw -lpthread
SRC=$(wildcard *.cc)
OBJ=$(SRC:.cc=.o)
HEADERS=$(wildcard *.h)
TARGET=nbtedit

%.o: %.cc $(HEADERS)
	$(CC) -c $(CFLAGS) -I. $< -o $@

all:
	$(MAKE) -C GZ/
	$(MAKE) -C NBT/
	$(MAKE) $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LIBS)

clean:
	-$(MAKE) clean -C GZ/
	-$(MAKE) clean -C NBT/
	-rm $(OBJ) $(TARGET)

