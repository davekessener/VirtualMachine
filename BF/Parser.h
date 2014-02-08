#ifndef PARSE_H
#define PARSE_H

#include <stdio.h>
#include <stdlib.h>

#define CMD_NONE 0
#define CMD_ADD 1
#define CMD_SUB 2
#define CMD_INC 3
#define CMD_DEC 4
#define CMD_BEGIN 5
#define CMD_END 6
#define CMD_READ 7
#define CMD_WRITE 8

class Parser
{
	public:
		Parser( );
		~Parser( );
		void parse(FILE *, FILE *);
	private:
		void indent( );
		void cmd(int);
		int c_type, c_count, idt;
		FILE *in, *out;
};

#endif

