CC=g++-4.7
CFLAGS=-std=c++0x -O0 -Wall -Wl,--no-as-needed -ggdb -DDEBUG
LIBS=-lreadline -lpthread -lz
SRC=$(wildcard *.cc)
OBJ=$(SRC:.cc=.o)
HEADERS=$(wildcard *.h)
TEMPLATES=$(wildcard *.hpp)
SUBS=
TARGET=lisp

%.o: %.cc $(HEADERS) $(TEMPLATES)
	$(CC) -c $(CFLAGS) -I. $< -o $@

%.a:
	$(MAKE) -C $(@:.a=)/
	cp $(@:.a=)/*.a .

%.clean:
	-$(MAKE) -C $(@:.clean=)/ clean

$(TARGET): logs $(SUBS) $(OBJ)
	$(CC) $(OBJ) -Wl,--start-group $(shell ls *.a) -Wl,--end-group -o $(TARGET) $(LIBS)

.PHONY: logs clean

logs:
	-mv *.log logs/
	-rm -f *.a

clean: logs $(SUBS:.a=.clean)
	-rm -f *.o $(TARGET)

