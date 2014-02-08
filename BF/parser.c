#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CMD_NONE 0
#define CMD_ADD 1
#define CMD_SUB 2
#define CMD_INC 3
#define CMD_DEC 4
#define CMD_WRITE 5
#define CMD_READ 6
#define CMD_LOOP 7
#define CMD_END 8

FILE *out, *in;
int idt;
int c_type, c_count;

int parse(FILE *, FILE*);
void cmd(int);
void printHeader();
void printFooder();
void indent();
int isComment(int);

int main(int argc, char *argv[])
{
	parse(stdin, stdout);

	return EXIT_SUCCESS;
}

int parse(FILE *_in, FILE *_out)
{
	int read_char = 0;

	in = _in;
	out = _out;
	idt = 1;
	c_type = CMD_NONE;
	c_count = 0;

	printHeader();

	while(read_char != EOF)
	{
		read_char = getc(in);
		
//		if(read_char == ';') while(read_char != '\n' && read_char != EOF) read_char = getc(in);

		if(isComment(read_char)) continue;

		switch(read_char)
		{
			case '+': cmd(CMD_ADD); break;
			case '-': cmd(CMD_SUB); break;
			case '>': cmd(CMD_INC); break;
			case '<': cmd(CMD_DEC); break;
			case '.': cmd(CMD_WRITE); break;
			case ',': cmd(CMD_READ); break;
			case ']': cmd(CMD_END); break;
			case '[': cmd(CMD_LOOP); break;
		}
	}

	cmd(CMD_NONE);

	putc('\n', out);

	indent();

	fprintf(out, "return 0;\n}\n");

	printFooder();

	return 0;
}

void _cmd(int);

void cmd(int c)
{
	_cmd(c <= CMD_DEC ? c : CMD_NONE);

	if(c > CMD_DEC)
	{
		if(c == CMD_END) idt--;

		indent();

		switch(c)
		{
			case CMD_WRITE:
				fprintf(out, "putc(*ptr, out);");
				break;
			case CMD_READ:
				fprintf(out, "*ptr = getc(in);");
				break;
			case CMD_LOOP:
				fprintf(out, "while(*ptr)\n");
				indent();
				putc('{', out);
				idt++;
				break;
			case CMD_END:
				putc('}', out);
				break;
		}

		putc('\n', out);
	}
}

void _cmd(int c)
{
	if(c_type == c)
	{
		if(c_type != CMD_NONE) c_count++;
	}
	else
	{
		if(c_type > 0 && c_count > 0)
		{
			char ac = '+', vt = '*';

			if(c_type == CMD_SUB || c_type == CMD_DEC) ac = '-';
			if(c_type == CMD_INC || c_type == CMD_DEC) vt = '\0';
			
			indent();

			if(c_count > 1)
			{
				if(vt) putc(vt, out);
				fprintf(out, "ptr %c= %d;\n", ac, c_count);
			}
			else
			{
				fprintf(out, "%c%c", ac, ac);
				if(vt) putc(vt, out);
				fprintf(out, "ptr;\n");
			}
		}

		c_type = c;
		c_count = c ? 1 : 0;
	}
}

void indent(void)
{
	int i = idt; while(i--) putc('\t', out);
}

void printHeader(void)
{
	fprintf(out,
		"#include <stdio.h>\n"
		"#include <stdlib.h>\n"
		"#include <string.h>\n\n"
		"#define CHUNK 0x4000\n\n"
		"void openFiles(int, char **, FILE **, FILE **);\n\n"
		"int main(int argc, char *argv[])\n{\n"
		"\tFILE *in, *out;\n"
		"\tunsigned char *ptr;\n\n"
		"\topenFiles(argc, argv, &in, &out);\n\n"
		"\tptr = malloc(CHUNK);\n\n"
		"\tmemset(ptr, 0, CHUNK);\n\n");
}

void printFooder(void)
{
	fprintf(out,
		"\nvoid openFiles(int argc, char *argv[], FILE **in, FILE **out)\n{\n"
		"\t*in = stdin;\n"
		"\t*out = stdout;\n"
		"}\n");
}

int isComment(int c)
{
	const char *cmp = "-+<>,.[]";
	int i;

	for(i = strlen(cmp) - 1 ; i >= 0 ; --i)
	{
		if(cmp[i] == c) return 0;
	}

	return 1;
}

