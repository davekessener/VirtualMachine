#ifndef HAW_LIB_HSM_STATE_H
#define HAW_LIB_HSM_STATE_H

#include <map>
#include <vector>
#include <string>

namespace lib
{
	typedef int state_t;
	typedef int event_t;

	class State
	{
		public:
		static const state_t SELF = -1;

		public:
			virtual ~State( ) { }
			virtual state_t getNext(event_t) { return SELF; }
			virtual void exit( ) { }
			virtual void entry( ) { }
			virtual void transition(State *, event_t) { }
			virtual bool contains(const State *s) const { return s == this; }
		private:
	};

	class StateRegistry
	{
		typedef void (*trans_fn)(void);
		struct trans_t
		{
			trans_t(State *s1, event_t e, State *s2) : from(s1), signal(e), to(s2) { }
			bool operator==(const trans_t& t) const { return from == t.from && signal == t.signal && to == t.to; }
			bool operator!=(const trans_t& t) const { return !(*this == t); }
			bool operator<(const trans_t& t) const
				{ return from != t.from
							? (from < t.from) 
							: (signal != t.signal 
								? (signal < t.signal) 
								: (to != t.to 
									? to < t.to 
									: false)); }
			State *from;
			event_t signal;
			State *to;
		};

		public:
			~StateRegistry( );
			void addState(state_t id, State *s) { states_[id] = s; }
			void addTransition(state_t s1, event_t e, state_t s2, trans_fn f) 
				{ trans_[trans_t(get(s1), e, get(s2))] = f; }
			State *get(state_t id) { return states_.at(id); }
			void transition(State *s1, event_t e, State *s2) { (trans_.at(trans_t(s1, e, s2)))(); }
		private:
			std::map<state_t, State *> states_;
			std::map<trans_t, trans_fn> trans_;
	};

	class ParallelStates : public State
	{
		typedef std::vector<State *> vec_t;
		typedef vec_t::iterator iter_t;
		typedef vec_t::const_iterator citer_t;

		public:
			ParallelStates(StateRegistry *sr) : reg_(sr) { }
			void addState(state_t);
			void transition(State *, event_t);
			void exit( );
			void entry( );
			state_t getNext(event_t);
			bool contains(const State *) const;
		private:
			StateRegistry *reg_;
			vec_t states_;
	};

	class StateMachine : public State
	{
		typedef std::vector<State *> vec_t;
		typedef vec_t::iterator iter_t;
		typedef vec_t::const_iterator citer_t;

		public:
			StateMachine(StateRegistry *sr, state_t initial, bool h = false) 
				: initial_(initial), state_(sr->get(initial)), reg_(sr), hist_(h) { }
			void addState(state_t id) { states_.push_back(reg_->get(id)); }
			void signal(event_t);
			void transition(State *, event_t);
			void entry( );
			void exit( );
			state_t getNext(event_t e) { return state_->getNext(e); }
			bool contains(const State *) const;
		private:
			State *getSuperState(State *);
		private:
			state_t initial_;
			State *state_;
			StateRegistry *reg_;
			vec_t states_;
			bool hist_;
	};
}

#endif

