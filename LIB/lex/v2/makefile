.PHONY: all clean

all:
	./make.sh

clean:
	./clean.sh

test:
	g++-4.7 -std=gnu++11 -O3 -Wall -ggdb -I/home/dave/include out.cpp Tokenizer.cc.o -o test -L/home/dave/include/lib -lboost_system -lboost_regex

