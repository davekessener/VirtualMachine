CC=g++
CFLAGS=-std=gnu++11 -O0 -Wall -Wl,--no-as-needed -gdwarf-3 -ggdb -DDEBUG
SRC=$(wildcard *.cc)
OBJ=$(SRC:.cc=.o)
HEADERS=$(wildcard *.h)
TEMPLATES=$(wildcard *.hpp)

%.o: %.cc $(HEADERS) $(TEMPLATES)
	$(CC) -c $(CFLAGS) $< -o $@

%.clean:
	-$(MAKE) -C $(@:.clean=)/ clean

$(TARGET): $(SUBS) $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

.PHONY: clean

clean:
	rm -f *.o $(TARGET)

