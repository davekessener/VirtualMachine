TEMPS=$(wildcard *.hpp)

.PHONY: logs clean

all: logs test parser

test: test.cc lex.h $(TEMPS)
	g++-4.7 -std=gnu++11 -O0 -ggdb -Wall -I/home/dave/include test.cc -o test -L/home/dave/include/lib -ldav_Logger -lboost_system -lboost_regex

parser: parser.cc lex.h $(TEMPS)
	g++-4.7 -std=gnu++11 -O0 -ggdb -Wall -I/home/dave/include parser.cc -o parser -L/home/dave/include/lib -ldav_Logger -lboost_system -lboost_regex

autogen: autogen.cc lex.h $(TEMPS)
	g++-4.7 -std=gnu++11 -O0 -ggdb -Wall -I/home/dave/include autogen.cc -o autogen -L/home/dave/include/lib -ldav_Logger -lboost_system -lboost_regex

logs:
	rm -f *.log

clean: logs
	rm test

