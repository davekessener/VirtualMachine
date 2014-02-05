#define A_IMP
#include "A.h"
#undef A_IMP

template<BYTE ID>
void A<ID>::init(void)
{
	v_t::clear();
	v_t::push_back("noooooooo!!!!!!");
}

template class A<3>;

