#ifndef BRAINFUCK_H
#define BRAINFUCK_H

#include <iostream>
#include <cstdint>
#include <cstring>
#include <cstdlib>

#define CHUNK 0x4000

typedef uint8_t BYTE;

class BrainFuck
{
	public:
		BrainFuck( );
		BrainFuck(int, char **);
		~BrainFuck( );
		BYTE& operator*( );
		BrainFuck& operator++( );
		BrainFuck& operator--( );
		void readByte( );
		void writeByte( );
	private:
		void init( );
		BrainFuck(const BrainFuck&);
		BrainFuck(BrainFuck&&);
		BrainFuck& operator=(const BrainFuck&);
		BrainFuck& operator=(BrainFuck&&);
		int c, idx;
		BYTE **chunks;
		BYTE *ptr;
		std::istream *inF;
		std::ostream *outF;
};

#endif

