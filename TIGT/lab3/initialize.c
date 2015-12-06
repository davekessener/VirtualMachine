#include "header.h"

extern void TI_Init_Board(void);
extern void initHW(void);

state_t state_init(void);
state_t state_running(void);
state_t state_hold(void);

void setled(led_t, bool_t);
void displaytime(time_t);
time_t checktimer(void);

extern state_fn states[3];
extern time_t time;
extern state_t state;

void initialize(void)
{
	TI_Init_Board();
	initHW();

	states[STATE_INIT] = state_init;
	states[STATE_RUNNING] = state_running;
	states[STATE_HOLD] = state_hold;

	setled(LED_D19, OFF);
	setled(LED_D20, OFF);

	state = STATE_INIT;
	time = 0;

	checktimer();
	displaytime(0);
}

