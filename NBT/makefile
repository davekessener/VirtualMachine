CC=g++-4.7
INCDIR=-I$(shell echo ~/include)
LIBDIR=-L$(shell echo ~/include/lib)
CFLAGS=-std=c++0x -Wl,--no-as-needed -ggdb -DDEBUG $(INCDIR)
DAVLIB=-ldav_nbt -ldav_gzstream
LIBS=$(LIBDIR) $(DAVLIB) -lncursesw -lpthread
SRC=$(wildcard *.cc)
OBJ=$(SRC:.cc=.o)
HEADERS=$(wildcard *.h)
TARGET=nbtedit

%.o: %.cc $(HEADERS)
	$(CC) -c $(CFLAGS) -I. $< -o $@

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LIBS)

clean:
	rm -r *.o $(TARGET)

