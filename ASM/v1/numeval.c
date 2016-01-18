#include "numeval.h"

void evalStack(const char *);
DS *getStack();
void printChar(char);

int executeNumberEvaluator(const char *cmd)
{
	int r;
	DS *stack = getStack(); 

	DS_init(stack);

	*MAX_DIGITS() = 21;

	setEvalOutput(evalStack);

	if(!evaluate(cmd))
	{
		assert(stack!=NULL&&stack->data!=NULL&&stack->i>0);

		r = stack->data[stack->i - 1];
	}
	else
	{
		fprintf(stderr, "ERR: Couldn't evaluate '%s'!\nAbort.\n", cmd);
		exit(1);
	}
	
	DS_dispose(stack);
	
	return r;
}

void evalStack(const char *line)
{
	DLOG("%s\n", line);
	interpret(getStack(), line);	
}

DS *getStack(void)
{
	static DS stack;

	return &stack;
}
