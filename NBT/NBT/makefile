CC=g++
CFLAGS=-fPIC -std=c++0x
SRC=$(wildcard *.cc)
OBJ=$(SRC:.cc=.o)
LIBS=-L../GZ -lgz
TARGET=libnbt.so

%.o: %.cc $(TEMPLATES)
	$(CC) -c $(CFLAGS) -I.. $< -o $@

all: $(OBJ)
	$(CC) -shared -Wl,-soname,$(TARGET) -o $(TARGET) $(OBJ) $(LIBS)

clean:
	-rm *.o $(TARGET)

