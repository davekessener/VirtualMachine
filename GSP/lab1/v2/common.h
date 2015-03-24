#ifndef DAV_GSP_1_COMMON_H
#define DAV_GSP_1_COMMON_H

#include <stdint.h>

#define MXT_USE_INT (1)
#define MXT_USE_DOUBLE (2)

// # ===========================================================================
#define MXT_USENUM MXT_USE_DOUBLE
// # ===========================================================================

#ifndef FALSE
#	define FALSE 0
#endif

#ifndef TRUE
#	define TRUE (!FALSE)
#endif

typedef unsigned uint;
typedef uint8_t BYTE;
typedef uint16_t WORD;
typedef uint32_t DWORD;
typedef uint64_t QWORD;

#define ERR_NO 0
#define ERR_STACK_FULL 1
#define ERR_STACK_EMPTY 2
#define ERR_IO_NONUM 3
#define ERR_IO_MALFORMED 4
#define ERR_MAIN_UNKNOWN 5
#define ERR_UNKNOWN 6

#define MXT_STACKCAP 0x400

#if MXT_USENUM == MXT_USE_INT
#	define MXT_STYPE int
#	define MXT_STYPEP "%d"
#elif MXT_USENUM == MXT_USE_DOUBLE
#	define MXT_STYPE double
#	define MXT_STYPEP "%.3lf"
#else
#	define MXT_STYPE void
#endif

#endif

