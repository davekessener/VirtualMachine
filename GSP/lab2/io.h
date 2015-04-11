#ifndef DAV_GSP_2_IO_H
#define DAV_GSP_2_IO_H

#include "common.h"

void io_init(void);

int io_get_sensors(void);
int io_get_reset(void);
int io_get_error_reset(void);

void io_set_counter(uint);
void io_set_fwd_led(int);
void io_set_rotation(int);
void io_set_error(int);

#endif

