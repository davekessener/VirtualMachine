#include "BrainFuck.h"

BrainFuck::BrainFuck(void) : inF(&std::cin), outF(&std::cout)
{
	init();
}

BrainFuck::BrainFuck(int argc, char *argv[]) : inF(&std::cin), outF(&std::cout)
{
	init();
}

BrainFuck::~BrainFuck(void)
{
	for(int i = 0 ; i < c ; i++)
	{
		delete[] chunks[i];
	}

	std::free(chunks);
}

void BrainFuck::init(void)
{
	c = 1; idx = 0;
	chunks = reinterpret_cast<BYTE **>(std::malloc(sizeof(BYTE *)));
	ptr = chunks[0] = new BYTE[CHUNK];
}

BYTE& BrainFuck::operator*(void)
{
	return *ptr;
}

BrainFuck& BrainFuck::operator++(void)
{
	if(++ptr == chunks[idx] + CHUNK)
	{
		if(++idx == c)
		{
			c <<= 1;
			chunks = reinterpret_cast<BYTE **>(std::realloc(chunks, c * sizeof(BYTE *)));
		}

		ptr = chunks[idx];
	}
}

BrainFuck& BrainFuck::operator--(void)
{
	if(ptr-- == chunks[idx])
	{
		if(!idx--)
		{
			std::cerr << "ERR: Invalid pointer movement." << std::endl;
			std::cerr << "Tried to drop below 0." << std::endl;
			std::exit(1);
		}

		ptr = chunks[idx];
	}
}

void BrainFuck::readByte(void)
{
	*ptr = inF->get();
}

void BrainFuck::writeByte(void)
{
	outF->put(*reinterpret_cast<char *>(ptr));
}

