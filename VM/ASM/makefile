CC=g++-4.7
CFLAGS=-Wall -std=c++0x -O0 -Wl,--no-as-needed -ggdb -DDEBUG
LIBS=
SRC=$(wildcard *.cc)
OBJ=$(SRC:.cc=.o)
HEADERS=$(wildcard *.h)
TEMPLATES=$(wildcard *.hpp)
SUBS=
TARGET=dasm

%.o: %.cc $(HEADERS) $(TEMPLATES)
	$(CC) -c $(CFLAGS) -I. -I.. $< -o $@

%.a:
	$(MAKE) -C $(@:.a=)/
	cp $(@:.a=)/*.a .

%.clean:
	-$(MAKE) -C $(@:.clean=)/ clean

$(TARGET): logs $(SUBS) $(OBJ)
	$(CC) $(OBJ) -Wl,--start-group $(shell find -maxdepth 1 -name "*.a") -Wl,--end-group -o $(TARGET) $(LIBS)

.PHONY: logs clean

logs:
	for file in *.log; do if [ -f "$$file" ]; then mv "$$file" logs/; fi; done
	rm -f *.a

clean: logs $(SUBS:.a=.clean)
	rm -f *.o $(TARGET)

