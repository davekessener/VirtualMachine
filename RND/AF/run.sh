#!/bin/bash
./parser -p < tfill.txt > TableFill.cc
sed -i -e 's/^\/\///' -e 's/[ \t]\+#/\/\/&/' TableFill.cc
g++ -std=gnu++11 -Wall -c -I ~/include -I ~/include/lex/final/ TableFill.cc -o TableFill.cc.o
g++ TableFill.cc.o Tokenizer.cc.o -o TableFill -L ~/include/lib -lboost_regex

