.PHONY: logs clean

test: test.cc lex.h logs
	g++-4.7 -std=gnu++11 -O0 -ggdb -Wall -I/home/dave/include test.cc -o test -L/home/dave/include/lib -ldav_Logger -lboost_system -lboost_regex

logs:
	rm -f *.log

clean: logs
	rm test

