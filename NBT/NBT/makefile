CC=g++
CFLAGS=-fPIC -std=c++0x
DEBUG=-ggdb -DDEBUG
SRC=$(wildcard *.cc)
OBJ=$(SRC:.cc=.o)
LIBS=-L../GZ -lgz
TARGET=libnbt.so

%.o: %.cc %.h $(TEMPLATES)
	$(CC) -c $(CFLAGS) $(DEBUG) -I.. $< -o $@

$(TARGET): $(OBJ)
	$(CC) -shared -Wl,-soname,$(TARGET) -o $(TARGET) $(OBJ) $(LIBS)

clean:
	-rm *.o $(TARGET)

