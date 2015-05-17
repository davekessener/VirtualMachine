#include "calc.h"

CELL *new_cell(void)
{
	CELL *c = malloc(sizeof(*c));

	c->id = C_NONE;
	c->left = c->right = NULL;

	return c;
}

#define MXT_BUFSIZE 0x400
char sbuf[MXT_BUFSIZE + 1];

void printcell(CELL *c, size_t ident)
{
	const char *is = sbuf + MXT_BUFSIZE - ident;

	printf("%s", is);

	switch(c->id)
	{
		case C_NONE:
			printf("ERROR\n");
			break;
		case C_NUM:
			printf("NUM (%lf)\n", c->data.num);
			break;
		case C_VAR:
			printf("VAR (%c)\n", c->data.id + 'a');
			if(c->right) printcell(c->right, ident + 4);
			break;
		case C_ASSIGN:
			printf("ASSIGN\n");
			printcell(c->left, ident + 4);
			printf("%s  =\n", is);
			printcell(c->right, ident + 4);
			break;
		case C_EXPRLIST:
			printf("LIST\n");
			printcell(c->left, ident + 4);
			if(c->right) printcell(c->right, ident);
			break;
		case C_BINOP:
			printf("BINOP (%c)\n", c->data.binop);
			printcell(c->left, ident + 4);
			printf("%s  %c\n", is, c->data.binop);
			printcell(c->right, ident + 4);
			break;
		case C_NEG:
			printf("NEG\n");
			printcell(c->right, ident + 4);
			break;
		case C_CALL:
			printf("CALL (%c)\n", c->data.id + 'a');
			printcell(c->right, ident + 4);
			break;
	}
}

const char *generate_lisp(CELL *c)
{
	static char *fs;
	char buf[MXT_BUFSIZE];

	if(fs) { free(fs); fs = NULL; }

	if(!c) return fs = strdup("");

	switch(c->id)
	{
		case C_NONE:
			fprintf(stderr, "ERR: Empty cell.!\n");
			exit(1);
			break;
		case C_NUM:
			snprintf(buf, MXT_BUFSIZE, "%g", c->data.num);
			fs = strdup(buf);
			break;
		case C_VAR:
			snprintf(buf, MXT_BUFSIZE, "%c", c->data.id + 'a');
			fs = strdup(buf);
			break;
		case C_ASSIGN:
			{
				int f = 0;
				if(c->left->id == C_CALL)
				{
					snprintf(buf, MXT_BUFSIZE, "(defun %c (", c->left->data.id + 'a');
					strcat(buf, generate_lisp(c->left->right));
					strcat(buf, ") ");
				}
				else
				{
					snprintf(buf, MXT_BUFSIZE, "(setq '%c ", c->left->data.id + 'a');
				}
				strcat(buf, generate_lisp(c->right));
				strcat(buf, ")");
				free(fs);
				fs = strdup(buf);
			}
			break;
		case C_EXPRLIST:
			{
				CELL *p = c;
				buf[0] = '\0';
				int f = 0;
				while(p)
				{
					if(f) strcat(buf, " "); f = 1;
					strcat(buf, generate_lisp(p->left));
					p = p->right;
				}
				free(fs);
				fs = strdup(buf);
			}
			break;
		case C_BINOP:
			snprintf(buf, MXT_BUFSIZE, "(%c ", c->data.binop);
			strcat(buf, generate_lisp(c->left));
			strcat(buf, " ");
			strcat(buf, generate_lisp(c->right));
			strcat(buf, ")");
			free(fs);
			fs = strdup(buf);
			break;
		case C_NEG:
			snprintf(buf, MXT_BUFSIZE, "(- %s)", generate_lisp(c->right));
			free(fs);
			fs = strdup(buf);
			break;
		case C_CALL:
			snprintf(buf, MXT_BUFSIZE, "(%c %s)", c->data.id + 'a', generate_lisp(c->right));
			free(fs);
			fs = strdup(buf);
			break;
	}

	return fs;
}

void eval(CELL *c)
{
	memset(sbuf, ' ', MXT_BUFSIZE);
	sbuf[MXT_BUFSIZE] = '\0';
//	printcell(c, 0);
	printf("%s\n", generate_lisp(c));
}

CELL *assign(CELL *f, CELL *b)
{
	CELL *c = new_cell();

	c->id = C_ASSIGN;
	c->left = f;
	c->right = b;

	return c;
}

CELL *fncall(uint v, CELL *args)
{
	CELL *c = new_cell();

	c->id = C_CALL;
	c->data.id = v;
	c->right = args;

	return c;
}

CELL *function(uint v, CELL *vars)
{
	return var_list(v, vars);
}

CELL *number(double d)
{
	CELL *c = new_cell();

	c->id = C_NUM;
	c->data.num = d;

	return c;
}

CELL *variable(uint v)
{
	CELL *c = new_cell();

	c->id = C_VAR;
	c->data.id = v;

	return c;
}

CELL *var_list(uint v, CELL *cdr)
{
	CELL *c = variable(v);

	c->right = cdr;

	return c;
}

CELL *expr_list(CELL *e, CELL *cdr)
{
	CELL *c = new_cell();

	c->id = C_EXPRLIST;
	c->left = e;
	c->right = cdr;

	return c;
}

CELL *append(CELL *car, CELL *cdr)
{
	CELL *c = car;

	if(!car) return cdr;

	while(car->right) car = car->right;

	car->right = cdr;

	return c;
}

CELL *op_neg(CELL *e)
{
	CELL *c = new_cell();

	c->id = C_NEG;
	c->right = e;

	return c;
}

CELL *binop(char op, CELL *l, CELL *r)
{
	CELL *c = new_cell();

	c->id = C_BINOP;
	c->data.binop = op;

	c->left = l;
	c->right = r;

	return c;
}

CELL *op_add(CELL *l, CELL *r) { return binop('+', l, r); }
CELL *op_sub(CELL *l, CELL *r) { return binop('-', l, r); }
CELL *op_mul(CELL *l, CELL *r) { return binop('*', l, r); }
CELL *op_div(CELL *l, CELL *r) { return binop('/', l, r); }
CELL *op_exp(CELL *l, CELL *r) { return binop('^', l, r); }


