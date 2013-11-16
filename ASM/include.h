/* Header for include.c that contains auxiliary function
 * used to read/print numbers
 * File: include.h for include.c
 * Daniel Kessener, HAW
 */
#ifndef __INCLUDE_H
#define __INCLUDE_H

#define BUF_SIZE 1024

// Macro for string insertion.
// Currently unused.
#define STR(x) # x
#define STRX(x) STR(x)

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <assert.h>

// Default to using long doubles if there
// was no number-type predefined with the
// compiler-call
#ifndef FQP
#define FQP long double 
#endif

// Default maximum amount of printed digits
int *MAX_DIGITS(void);

// Defines PI and E globally
#define PI 3.1415926535897932384626433832795028841971693993751058
#define E  2.7182818284590452353602874713526624977572470936999595

// Reads a number from a string, updates pointer
// position.
//FQP evalNumber(const char**);
// Print a number in the specified base
void printNumber(FQP, int, void (*)(char));

#endif

