#include <iostream>
#include <stdio.h>

#include "State.h"

#define STATE_A1 1
#define STATE_A2 2
#define STATE_B1 3
#define STATE_B2 4
#define STATE_B3 5

#define EVENT_A 1
#define EVENT_B 2
#define EVENT_C 3
#define EVENT_D 4

using namespace lib;

struct A1 : public StateMachine
{
	A1(StateRegistry *sr, state_t i, bool h) : StateMachine(sr, i, h) { }
	void entry( ) { printf("A1: entry\n"); StateMachine::entry(); }
	void exit( ) { StateMachine::exit(); printf("A1: exit\n"); }
};

struct A2 : public StateMachine
{
	A2(StateRegistry *sr, state_t i, bool h) : StateMachine(sr, i, h) { }
	void entry( ) { printf("A2: entry\n"); StateMachine::entry(); }
	void exit( ) { StateMachine::exit(); printf("A2: exit\n"); }
	state_t getNext(event_t e) { return e == EVENT_D ? STATE_A1 : State::SELF; }
};

struct B1 : public State
{
	void entry( ) { printf("B1: entry\n"); }
	void exit( ) { printf("B1: exit\n"); }
	state_t getNext(event_t e) { return e == EVENT_A ? STATE_B2 : State::SELF; }
};

struct B2 : public State
{
	void entry( ) { printf("B2: entry\n"); }
	void exit( ) { printf("B2: exit\n"); }
	state_t getNext(event_t e) { return e == EVENT_B ? STATE_B1 : (e == EVENT_C ? STATE_B3 : State::SELF); }
};

struct B3 : public State
{
	void entry( ) { printf("B3: entry\n"); }
	void exit( ) { printf("B3: exit\n"); }
};

void b1_a_b2(void)
{
	printf("b1_a_b2\n");
}

void b2_b_b1(void)
{
	printf("b2_b_b1\n");
}

void b2_c_b3(void)
{
	printf("b2_c_b3\n");
}

void a2_d_a1(void)
{
	printf("a2_d_a1\n");
}

int main(int argc, char *argv[])
{
	StateRegistry reg;
	StateMachine *base, *a1, *a2;

	reg.addState(STATE_B1, new B1);
	reg.addState(STATE_B2, new B2);
	reg.addState(STATE_B3, new B3);
	reg.addState(STATE_A1, a1 = new A1(&reg, STATE_B1, true));
	reg.addState(STATE_A2, a2 = new A2(&reg, STATE_B3, true));
	base = new StateMachine(&reg, STATE_A1);

	a1->addState(STATE_B1);
	a1->addState(STATE_B2);
	a2->addState(STATE_B3);
	base->addState(STATE_A1);
	base->addState(STATE_A2);

	reg.addTransition(STATE_B1, EVENT_A, STATE_B2, &b1_a_b2);
	reg.addTransition(STATE_B2, EVENT_B, STATE_B1, &b2_b_b1);
	reg.addTransition(STATE_B2, EVENT_C, STATE_B3, &b2_c_b3);
	reg.addTransition(STATE_A2, EVENT_D, STATE_A1, &a2_d_a1);

	printf("A1: %p\nA2: %p\nB1: %p\nB2: %p\nB3: %p\n\n", a1, a2, reg.get(STATE_B1), reg.get(STATE_B2), reg.get(STATE_B3));

	printf("signal A:\n");
	base->signal(EVENT_A);
	printf("\nsignal B:\n");
	base->signal(EVENT_B);
	printf("\nsignal A:\n");
	base->signal(EVENT_A);
	printf("\nsignal C:\n");
	base->signal(EVENT_C);
	printf("\nsignal D:\n");
	base->signal(EVENT_D);
	printf("\nsignal B:\n");
	base->signal(EVENT_B);

	return 0;
}

