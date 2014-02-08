CC=g++
CFLAGS=-std=c++0x

all: BrainFuck.o parsed.o
	$(CC) $(CFLAGS) BrainFuck.o parsed.o -o parsed

BrainFuck.o: BrainFuck.cc BrainFuck.h
	$(CC) $(CFLAGS) -c BrainFuck.cc -o BrainFuck.o

parsed.o: parsed.cc
	$(CC) $(CFLAGS) -c parsed.cc -o parsed.o

parser: Parser.cc
	$(CC) $(CFLAGS) Parser.cc -o parser

clean:
	-rm *.o parsed

