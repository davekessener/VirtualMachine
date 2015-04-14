#include "main.h"
#include <stdio.h>
#include <string.h>
#include "TI_Lib.h"
#include "tft.h"
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"

#include "stack.h"
#include "io.h"
#include "keypad.h"

/* error routine */
void error(int);

/* operationen auf den stack */
int binop_f(int);
int p_f(void);
int f_f(void);
int c_f(void);
int d_f(void);
int r_f(void);

/* evaluierungsfunction */
/* parameter: current lookup */
int evaluate(int);

int main(int argc, char *argv[])
{  
	/* Initialisiere TI board und touchscreen */
    Init_TI_Board();
    TFT_cls();

    Make_Touch_Pad();

	/* evaluations-loop bis stdin leer ist */
	/* lookup(stdin) liesst einen character von
	   stdin. lookup(NULL) gibt den zuvor ein-
	   gelesenen character wieder */
	while(evaluate(lookup(stdin))) ;

	end:
		goto end;
}

int evaluate(int t)
{
	int err = 0;

	/* switch on lookup */
	switch(t)
	{
		case '+':
		case '-':
		case '*':
		case '/':
			/* binaere rechenoperation */
			/* wenn fehlerwert zurueckgegeben wird
			   wird die fehlerroutine aufgerufen */
			if((err = binop_f(t))) error(err);
			break;
		/* stack metafunctionen */
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
		/* EOS fuer stdin oder 'q' -> quit */
		case EOF:
			printf("Reached end of input.\n");
		case 'q':
			printf("Goodbye.\n");
			return FALSE;
		default:
			/* wenn lookup whitespace ist, ignorieren */
			if(isws(t)) ;
			/* andernfalls wenn lookup eine ziffer ist
			   zahl einlesen und auf den stack legen */
			else if(isnum(t))
			{
				uint v = 0;

				if((err = read_int(&v)) || (err = stack_push(v)))
				{
					error(err);
				}

				/* recursion damit der lookup nach einlesen
				   der zahl nicht in main ueberschrieben wird. */
				return evaluate(lookup(NULL));
			}
			/* andernfalls ist ein unbekannter befehl eingelesen worden */
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
#define MXT_ISOVER(a,b,v) (((a)<0&&(b)<0&&(v)>(a))||((a)>0&&(b)>0&&v<(a)))
			case '+':
				v = b + a;
                if(MXT_ISOVER(a,b,v)) return ERR_OVERFLOW;
				break;
			case '-':
				v = b - a;
                if(MXT_ISOVER(-a,b,v)) return ERR_OVERFLOW;
				break;
			case '*':
				v = b * a;
                if(b && a && v / a != b) return ERR_OVERFLOW;
				break;
			case '/':
                if(!a) return ERR_DIVZERO;
				v = b / a;
				break;
			default:
				return ERR_UNKNOWN;
#undef MXT_ISOVER
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
		printf("\n" MXT_STYPEP "\n", v);
        Clear_Line();
        Display_value((int) v);
        TFT_puts("\n\r");
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
		printf("\n" MXT_STYPEP "\n", v);
	}
    
    Clear_Line();

	return r;
}

int c_f(void)
{
	int r = ERR_NO;

	while(!stack_empty())
	{
		if((r = stack_pop(NULL))) break;
	}
    
    Clear_Line();

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
	printf("\nERR (%d)!\n", ec);
    
    Clear_Line();
    
    char buf[0x20];
    sprintf(buf, "ERR(%d)\n\r", ec);
    buf[0x1f] = '\0';
    TFT_puts(buf);
    
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
        case ERR_OVERFLOW:
            printf("Overflow error occurred.\n");
            break;
        case ERR_DIVZERO:
            printf("Tried to divide by zero.\n");
            break;
		default:
			printf("Error handler malfunction. Abort.\n");
        error:
            goto error;
	}
}

