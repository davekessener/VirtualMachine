#!/bin/bash
./parser < $1.b > $1.c
gcc $1.c -o $1_

