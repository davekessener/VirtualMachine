CC=g++-4.7
SRC=Timer.cc main.cc 
OBJ=Timer.cc.o main.cc.o 
CFLAGS=
LIBS=
INCDIRS=
LIBDIRS=

TARGET=lab2
LIBS=-lpthread

ACFLAGS=$(CFLAGS) -std=gnu++11 -Wall -O0 -ggdb -DDEBUG
ALIBS=$(LIBS) 
AINCDIRS=$(INCDIRS)
ALIBDIRS=$(LIBDIRS)

.PHONY: logs clean

all: logs $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(ALIBDIRS) -Wl,--start-group $(ALIBS) -Wl,--end-group

Timer.cc.o: Timer.cc Timer.h
	$(CC) -c $(ACFLAGS) $(AINCDIRS) Timer.cc -o Timer.cc.o

main.cc.o: main.cc Insertion.hpp Merge.hpp Selection.hpp Sorting.hpp Stack.hpp Timer.h
	$(CC) -c $(ACFLAGS) $(AINCDIRS) main.cc -o main.cc.o

logs:
	if [ -d "logs" ]; then for LF in *.log; do if [ -f "$$LF" ]; then mv $$LF logs/; fi; done; fi

clean: logs
	rm -f $(OBJ) $(TARGET)

