#include "header.h"

extern void initialize(void);
extern time_t checktimer(void);

state_fn states[3];
time_t time;
state_t state;

int main(int argc, char *argv[])
{
	initialize();

	while(TRUE)
	{
		time += checktimer();

		state = states[state]();
	}
}

