#ifndef VM_ASM_ADEF_H
#define VM_ASM_ADEF_H

#include <ddef.h>

#define MXT_LOGANDTHROW(...) ::Logger::logFrom(__FILE__, __LINE__, __VA_ARGS__), throw ASMException(__VA_ARGS__)

#endif

