#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	FILE *in = stdin, *out = stdout;
	int c;
	int indent = 0, i;

	fprintf(out, "#include \"BrainFuck.h\"\n\nint main(int argc, char *argv[])\n{\n"
				 "\tBrainFuck bf(argc, argv);\n\n");

	indent++;

	while(1)
	{
		c = getc(in);

idt:
		i = indent; while(i--) putc('\t', out);

		if(c == EOF) break;

		switch(c)
		{
			case '+': fprintf(out, "++*bf;"); break;
			case '-': fprintf(out, "--*bf;"); break;
			case '>': fprintf(out, "++bf;"); break;
			case '<': fprintf(out, "--bf;"); break;
			case '[': fprintf(out, "while(*bf)\n"); c = 0xffff; goto idt;
			case ']': putc('}', out); indent--; break;
			case '.': fprintf(out, "bf.writeByte();"); break;
			case ',': fprintf(out, "bf.readByte();"); break;
			case 0xffff: putc('{', out); indent++; break;
		};

		putc('\n', out);
	}

	fprintf(out, "return 0;\n}\n");

	return EXIT_SUCCESS;
}

