#!/bin/bash
make
./parser -v < equations.txt > out.log
cat out.log

