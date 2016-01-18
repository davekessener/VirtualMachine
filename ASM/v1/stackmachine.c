#define SM_OPS
#include "stackmachine.h"
#undef SM_OPS

int  interpret(DS *stack, const char *input)
{
	int i;
	char buf[1024];
	const char *tmp;
	FQP v1, v2;
	long long t;

	if(!*input) return SM_ERR_INPUT;

	sscanf(input, " %s", buf);

#define BEQ(i) (strcmp(buf, SM_OP_Instructions(i))==0)
	if(BEQ(SM_OP_PUSH))
	{
		i = 4; while(input[i] == ' ' || input[i] == '\t') i++;
		tmp = input + i; v1 = evalNumber((char **) &tmp);

		DS_push(stack, v1);
	}
	else 
	{
		if(stack->c <= 0) return SM_ERR_EMPTY;
		v1 = DS_pop(stack);

		if(BEQ(SM_OP_NEG))
		{
			DS_push(stack, -v1);
		}
		else if(BEQ(SM_OP_SIN))
		{
			while(v1 > 2.0 * PI) v1 -= 2.0 * PI;
			while(v1 < 0.0) v1 += 2.0 * PI;
			DS_push(stack, (FQP) sin((double) v1));
		}
		else if(BEQ(SM_OP_COS))
		{
			while(v1 > 2.0 * PI) v1 -= 2.0 * PI;
			while(v1 < 0.0) v1 += 2.0 * PI;
			DS_push(stack, (FQP) cos((double) v1));
		}
		else if(BEQ(SM_OP_TAN))
		{
			while(v1 > 2.0 * PI) v1 -= 2.0 * PI;
			while(v1 < 0.0) v1 += 2.0 * PI;
			DS_push(stack, (FQP) tan((double) v1));
		}
		else if(BEQ(SM_OP_LOG) || BEQ(SM_OP_LG))
		{
			DS_push(stack, (FQP) log10((double) v1));
		}
		else if(BEQ(SM_OP_LN))
		{
			DS_push(stack, (FQP) log((double) v1));
		}
		else if(BEQ(SM_OP_RUP))
		{
			v2 = v1 < 0.0L ? -1.0L : 1.0L; v1 = v1 < 0.0L ? -v1 : v1;
			DS_push(stack, v2 * ((FQP) ((long long) (v1 + 0.999999L))));
		}
		else if(BEQ(SM_OP_RDWN))
		{
			v2 = v1 < 0.0L ? -1.0L : 1.0L; v1 = v1 < 0.0L ? -v1 : v1;
			DS_push(stack, v2 * ((FQP) ((long long) v1)));
		}
		else if(BEQ(SM_OP_RND))
		{
			v2 = v1 < 0.0L ? -1.0L : 1.0L; v1 = v1 < 0.0L ? -v1 : v1;
			DS_push(stack, v2 * ((FQP) ((long long) (v1 + 0.5L))));
		}
		else if(BEQ(SM_OP_XSUM))
		{
			t = (long long) (v1 < 0.0L ? -v1 : v1);
			v1 = 0.0L;
			while(t > 0)
			{
				v1 += t % 10;
				t /= 10;
			}
			DS_push(stack, v1);
		}
		else if(BEQ(SM_OP_LNOT))
		{
			DS_push(stack, !v1);
		}
		else if(BEQ(SM_OP_NOT))
		{
			DS_push(stack, ~((long long) v1));
		}
		else
		{
			if(stack->c < 1) return SM_ERR_EMPTY;
			v2 = DS_pop(stack);

			if(BEQ(SM_OP_ADD))
			{
				DS_push(stack, v2 + v1);
			}
			else if(BEQ(SM_OP_SUB))
			{
				DS_push(stack, v2 - v1);
			}
			else if(BEQ(SM_OP_MUL))
			{
				DS_push(stack, v2 * v1);
			}
			else if(BEQ(SM_OP_DIV))
			{
				DS_push(stack, v2 / v1);
			}
			else if(BEQ(SM_OP_EXP))
			{
				DS_push(stack, (FQP) pow((double) v2, (double) v1));
			}
			else if(BEQ(SM_OP_LAND))
			{
				DS_push(stack, (FQP) (v1 && v2));
			}
			else if(BEQ(SM_OP_LOR))
			{
				DS_push(stack, (FQP) (v1 || v2));
			}
			else if(BEQ(SM_OP_AND))
			{
				DS_push(stack, (FQP) ((long long)v1 & (long long)v2));
			}
			else if(BEQ(SM_OP_OR))
			{
				DS_push(stack, (FQP) ((long long)v1 | (long long)v2));
			}
			else if(BEQ(SM_OP_XOR))
			{
				DS_push(stack, (FQP) ((long long)v1 ^ (long long)v2));
			}
			else if(BEQ(SM_OP_MOD))
			{
				DS_push(stack, (FQP) ((long long)v2 % (long long)v1));
			}
			else if(BEQ(SM_OP_SHL))
			{
				DS_push(stack, (FQP) ((long long)v2 << (long long)v1));
			}
			else if(BEQ(SM_OP_SHR))
			{
				DS_push(stack, (FQP) ((long long)v2 >> (long long)v1));
			}
			else
			{
				return SM_ERR_UNKNOWN;
			}
		}
	}

	return SM_ERR_NONE;
#undef BEQ
}

