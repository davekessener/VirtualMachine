#include "header.h"

extern uint16_t *portin;

bool_t checkkey(key_t k)
{
	return ~*portin & k;
}

