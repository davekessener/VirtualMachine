#ifndef DAV_VM_COMMON_H
#define DAV_VM_COMMON_H

#include <cstdint>

namespace vm
{
	using SBYTE = std::int8_t;
	using SWORD = std::int16_t;
	using SDWORD = std::int32_t;
	using SQWORD = std::int64_t;

	using BYTE = std::uint8_t;
	using WORD = std::uint16_t;
	using DWORD = std::uint32_t;
	using QWORD = std::uint64_t;

	using uint = unsigned;
}

#endif

