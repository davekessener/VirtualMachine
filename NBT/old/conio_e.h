#ifndef __CONIO_E_H
#define __CONIO_E_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/select.h>
#include <termios.h>

void prepare_terminal();
void restore_terminal();
int kbhit();
int getch();

#endif

