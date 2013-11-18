#ifndef __INCLUDE_H
#define __INCLUDE_H

#define BUF_SIZE (1<<16)

#define STR(x) # x
#define STRX(x) STR(x)

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <assert.h>

#ifndef FQP
#define FQP long double 
#endif

int *MAX_DIGITS(void);

#define PI 3.1415926535897932384626433832795028841971693993751058
#define E  2.7182818284590452353602874713526624977572470936999595

void printNumber(FQP, int, void (*)(char));

#endif

