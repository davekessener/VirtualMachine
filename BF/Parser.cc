#include "Parser.h"

int main(int argc, char *argv[])
{
	Parser p;

	p.parse(stdin, stdout);

	return 0;
}

Parser::Parser(void) : c_type(CMD_NONE), c_count(0), idt(0)
{
}

Parser::~Parser(void)
{
}

void Parser::parse(FILE *_in, FILE *_out)
{
	int read_char;

	in = _in;
	out = _out;

	fprintf(out, "#include \"BrainFuck.h\"\n\n"
				 "int main(int argc, char *argv[])\n{\n\t"
				 "BrainFuck(argc, argv);\n\n");

	do
	{
		read_char = getc(in);

		if(read_char == ']') idt--;

		indent();

		if(read_char != EOF)
		{
			switch(read_char)
			{
				case '+': cmd(CMD_ADD); break;
				case '-': cmd(CMD_SUB); break;
				case '>': cmd(CMD_INC); break;
				case '<': cmd(CMD_DEC); break;
				case '.': cmd(CMD_NONE); fprintf(out, "bf.writeByte();"); break;
				case ',': cmd(CMD_NONE); fprintf(out, "br.readByte();"); break;
				case '[': cmd(CMD_NONE); fprintf(out, "while(*bf)\n"); indent(); putc('{', out); idt++; break;
				case ']': cmd(CMD_NONE); putc('}', out); break;
			}
		}
		else
		{
			cmd(CMD_NONE);
		}

		putc('\n', out);
	} while(read_char != EOF);

	indent();

	fprintf(out, "return 0;\n}\n");
}

void Parser::cmd(int c)
{
	if(c_type != c)
	{
		if(c_type > 0 && c_count > 0)
		{
			switch(c_type)
			{
				case CMD_ADD: fprintf(out, "bf.add(%d);\n", c_count); indent(); break;
				case CMD_SUB: fprintf(out, "bf.sub(%d);\n", c_count); indent(); break;
				case CMD_INC: fprintf(out, "bf.inc(%d);\n", c_count); indent(); break;
				case CMD_DEC: fprintf(out, "bf.dec(%d);\n", c_count); indent(); break;
			}
		}

		c_type = c;
		c_count = 0;
	}
	else
	{
		if(c_type != CMD_NONE) c_count++;
	}
}

void Parser::indent(void)
{
	for(int i = 0 ; i < idt ; i++) putc('\t', out);
}

