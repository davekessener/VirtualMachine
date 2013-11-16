#define DSTACK_C
#include "dstack.h"
#undef DSTACK_C

void DS_init(DS *this)
{
	this->data = NULL;
	this->c    = 0;
	this->i    = 0;
}

void DS_push(DS *this, FQP f)
{
	DS_resize(this);
	this->data[this->i++] = f;
}

FQP  DS_pop(DS *this)
{
	FQP f;

	assert(this->i > 0);

	f = this->data[--this->i];
	DS_resize(this);
	
	return f;
}

void DS_dispose(DS *this)
{
	free(this->data);

	DS_init(this);
}

// # ==========================================================================

void DS_resize(DS *this)
{
	int o = this->c;

	if(this->c == 0)
	{
		this->c = DS_INIT;
	}
	else if(this->i >= this->c - 1)
	{
		this->c <<= 1;
	}
	else if(this->c > DS_INIT && this->i < this->c >> 2)
	{
		this->c >>= 1;
	}

	if(o != this->c)
	{
		this->data = realloc(this->data, this->c * sizeof(FQP));
	}
}

