#ifndef DAV_INC_DEF_H
#define DAV_INC_DEF_H

#include <cstdint>
#include <cstddef>

#ifdef NULL
#undef NULL
#endif

#define NULL nullptr
typedef decltype(NULL) NULL_t;

typedef std::uint8_t  BYTE;
typedef std::uint16_t WORD;
typedef std::uint32_t DWORD;
typedef std::uint64_t QWORD;

#endif

