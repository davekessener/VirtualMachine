#ifndef SIM_SIM_H
#define SIM_SIM_H

#include <cstdint>

#ifdef NULL
#undef NULL
#endif

#define NULL nullptr
typedef decltype(nullptr) NULL_t;

namespace sim
{
	typedef uint32_t V_t;
}

#endif

