#ifndef DAV_GSP_2_COMMON_H
#define DAV_GSP_2_COMMON_H

#include <stdint.h>

typedef unsigned uint;
typedef uint8_t  BYTE;
typedef uint16_t WORD;
typedef uint32_t DWORD;
typedef uint64_t QWORD;

#define MXT_STATES (1<<2)

#define MXT_ERR_NONE 0
#define MXT_ERR_STEP 1

#define MXT_DIR_NONE 0
#define MXT_DIR_FWD  1
#define MXT_DIR_BCK  2

#define MXT_R_MIN 0
#define MXT_R_MAX 360
#define MXT_C_MAX 100 // TODO
#define MXT_SHIFT 100
#define MXT_C_TO_R(c) ((c)*((MXT_R_MAX-MXT_R_MIN)<<MXT_SHIFT)/MXT_C_MAX+(MXT_CMIN<<MXT_SHIFT))

#endif

