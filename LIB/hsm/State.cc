#include <stdio.h>
#include "State.h"

namespace lib {

StateRegistry::~StateRegistry(void)
{	
	for(std::map<state_t, State *>::iterator i1 = states_.begin(), i2 = states_.end() ; i1 != i2 ; ++i1)
	{
		delete i1->second;
	}
}

// # ===========================================================================

void ParallelStates::addState(state_t id)
{
	states_.push_back(reg_->get(id));
}

void ParallelStates::transition(State *s, event_t e)
{
	for(iter_t i1 = states_.begin(), i2 = states_.end() ; i1 != i2 ; ++i1)
	{
		if((*i1)->contains(s))
		{
			(*i1)->transition(s, e);
		}
	}

	throw std::string("in parallel transition: unknown state");
}

void ParallelStates::exit(void)
{
	for(iter_t i1 = states_.begin(), i2 = states_.end() ; i1 != i2 ; ++i1)
	{
		(*i1)->exit();
	}
}

void ParallelStates::entry(void)
{
	for(iter_t i1 = states_.begin(), i2 = states_.end() ; i1 != i2 ; ++i1)
	{
		(*i1)->entry();
	}
}

state_t ParallelStates::getNext(event_t e)
{
	state_t next = State::SELF;

	for(iter_t i1 = states_.begin(), i2 = states_.end() ; i1 != i2 ; ++i1)
	{
		if((next = (*i1)->getNext(e)) != State::SELF)
			break;
	}

	return next;
}

bool ParallelStates::contains(const State *s) const
{
	for(citer_t i1 = states_.begin(), i2 = states_.end() ; i1 != i2 ; ++i1)
	{
		if((*i1)->contains(s))
			return true;
	}

	return false;
}

// # ===========================================================================

void StateMachine::signal(event_t e)
{
	state_t next = getNext(e);

	if(next != State::SELF)
	{
		transition(reg_->get(next), e);
	}
}

void StateMachine::transition(State *s, event_t e)
{
	if(state_->contains(s))
	{
		state_->transition(s, e);
	}
	else
	{
		State *n = getSuperState(s);

		state_->exit();

		printf(">> %p %i %p\n", state_, e, s);
		reg_->transition(state_, e, n);

		(state_ = n)->entry();
	}
}

void StateMachine::exit(void)
{
	state_->exit();
}

void StateMachine::entry(void)
{
	if(!hist_)
		state_ = reg_->get(initial_);

	state_->entry();
}

State *StateMachine::getSuperState(State *s)
{
	for(iter_t i1 = states_.begin(), i2 = states_.end() ; i1 != i2 ; ++i1)
	{
		if((*i1)->contains(s))
			return *i1;
	}

	throw std::string("can't find superstate");
}

bool StateMachine::contains(const State *s) const
{
	for(citer_t i1 = states_.begin(), i2 = states_.end() ; i1 != i2 ; ++i1)
	{
		if((*i1)->contains(s))
			return true;
	}

	return false;
}

}

