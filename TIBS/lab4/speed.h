#ifndef TI3_BS_4_DAV_SPEED_H
#define TI3_BS_4_DAV_SPEED_H

#define MXT_DEFAULT_SPEED -1

#ifdef __KERNEL__
#	include <linux/types.h>
#else
#	include <stdint.h>
#endif

typedef void (*tzm_change_fn)(int64_t);

void tzm_initialize(int64_t, tzm_change_fn);
void tzm_pressed(int, uint64_t);
void tzm_cleanup(void);

#endif

