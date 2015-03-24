#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include "io.h"

void error(int);

int binop_f(int);
int p_f(void);
int f_f(void);
int c_f(void);
int d_f(void);
int r_f(void);

int evaluate(int);

int main(int argc, char *argv[])
{
	while(evaluate(lookup(stdin))) ;

	return EXIT_SUCCESS;
}

int evaluate(int t)
{
	int err = 0;

	switch(t)
	{
		case '+':
		case '-':
		case '*':
		case '/':
			if((err = binop_f(t))) error(err);
			break;
		case 'p':
			if((err = p_f())) error(err);
			break;
		case 'f':
			if((err = f_f())) error(err);
			break;
		case 'c':
			if((err = c_f())) error(err);
			break;
		case 'd':
			if((err = d_f())) error(err);
			break;
		case 'r':
			if((err = r_f())) error(err);
			break;
		case EOF:
			printf("Reached end of input.\n");
		case 'q':
			printf("Goodbye.\n");
			return FALSE;
		default:
			if(isws(t)) ;
			else if(isnum(t))
			{
				double v = 0.0;

				if((err = read_number(&v)) || (err = stack_push(v)))
				{
					error(err);
				}

				return evaluate(lookup(NULL));
			}
			else
			{
				error(ERR_MAIN_UNKNOWN);
			}
	}

	return TRUE;
}

int binop_f(int op)
{
	int r = ERR_NO;
	MXT_STYPE a = 0;
	MXT_STYPE b = 0;
	MXT_STYPE v = 0;

	if(!(r = stack_pop(&a)) && !(r = stack_pop(&b)))
	{
		switch(op)
		{
			case '+':
				v = b + a;
				break;
			case '-':
				v = b - a;
				break;
			case '*':
				v = b * a;
				break;
			case '/':
				v = b / a;
				break;
			default:
				return ERR_UNKNOWN;
		}

		r = stack_push(v);
	}

	return r;
}

int p_f(void)
{
	int r = ERR_NO;
	MXT_STYPE v = 0;

	if(!(r = stack_pop(&v)))
	{
		printf(MXT_STYPEP "\n", v);
	}

	return r;
}

int f_f(void)
{
	int r = ERR_NO;
	MXT_STYPE v = 0;

	while(!stack_empty())
	{
		if((r = stack_pop(&v))) break;
		printf(MXT_STYPEP "\n", v);
	}

	return r;
}

int c_f(void)
{
	int r = ERR_NO;

	while(!stack_empty())
	{
		if((r = stack_pop(NULL))) break;
	}

	return r;
}

int d_f(void)
{
	int r = ERR_NO;
	MXT_STYPE v = 0;

	if(!(r = stack_pop(&v)) && !(r = stack_push(v)))
	{
		r = stack_push(v);
	}

	return r;
}

int r_f(void)
{
	int r = ERR_NO;
	MXT_STYPE a = 0;
	MXT_STYPE b = 0;

	if(!(r = stack_pop(&a)) && !(r = stack_pop(&b)))
	{
		if(!(r = stack_push(a))) r = stack_push(b);
	}

	return r;
}

void error(int ec)
{
	printf("ERR (%d)!\n", ec);
	switch(ec)
	{
		case ERR_STACK_FULL:
			printf("The stack is full!\n");
			break;
		case ERR_STACK_EMPTY:
			printf("The stack is empty!\n");
			break;
		case ERR_IO_NONUM:
			printf("Expected a number.\n");
			break;
		case ERR_IO_MALFORMED:
			printf("Malformed number!\n");
			break;
		case ERR_MAIN_UNKNOWN:
			printf("This command is not recognized!\n");
			break;
		case ERR_UNKNOWN:
			printf("This error should never occur.\n");
			break;
		default:
			printf("Error handler malfunction. Abort.\n");
			exit(1);
	}
}

