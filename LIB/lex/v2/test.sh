#!/bin/bash
make
./lex -v < eq.txt > out.log
cat out.log

