#include <iostream>

template<typename T>
struct State
{
	typedef T Data;

	static void enter(void) { std::cout << "enter base" << std::endl; }
	static void leave(void) { std::cout << "leave base" << std::endl; }
};

template<class Origin, class Event, class Destination>
struct Rule
{
	static void apply(const Event&) { std::cout << "empty transition" << std::endl; }
};


struct A : public State<void>
{
	static void enter(void)
	{
		std::cout << "enter A" << std::endl;
	}
};

struct B : public State<void>
{
	static void enter(void)
	{
		std::cout << "enter B" << std::endl;
	}

	static void leave(void)
	{
		std::cout << "leave B" << std::endl;
	}
};

struct Ea { };
struct Eb { };

template<>
struct Rule<A, Ea, B>
{
	static void apply(const Ea& e)
	{
		std::cout << "A -a> B" << std::endl;
	}
};

//template<>
//struct Rule<A, Eb, A>
//{
//	static void apply(const Ea& e)
//	{
//		std::cout << "A -a> B" << std::endl;
//	}
//};

template<>
struct Rule<B, Ea, A>
{
	static void apply(const Ea& e)
	{
		std::cout << "B -a> A" << std::endl;
	}
};

template<>
struct Rule<B, Eb, A>
{
	static void apply(const Eb& e)
	{
		std::cout << "B -b> A" << std::endl;
	}
};


struct FSMBase
{
	virtual ~FSMBase() { }
	int get_state() { return state_; }
	void set_state(int state) { state_ = state; }
	int state_;
};

struct TransImpl_Eb_Nil : public virtual FSMBase
{
	virtual void process(const Eb& e)
	{
		throw new std::string("ERR: transition does not exist!");
	}
};

struct TransImpl_Eb_BA : public TransImpl_Eb_Nil
{
	using FSMBase::get_state;
	using FSMBase::set_state;

	virtual void process(const Eb& e)
	{
		if(get_state() == 1)
		{
			B::leave();
			Rule<B, Eb, A>::apply(e);
			A::enter();

			set_state(0);
		}
		else
		{
			TransImpl_Eb_Nil::process(e);
		}
	}
};

struct TransImpl_Eb_AA : public TransImpl_Eb_BA
{
	using FSMBase::get_state;
	using FSMBase::set_state;

	virtual void process(const Eb& e)
	{
		if(get_state() == 0)
		{
			Rule<A, Eb, A>::apply(e);

			set_state(0);
		}
		else
		{
			TransImpl_Eb_BA::process(e);
		}
	}
};

struct TransImpl_Ea_Nil : public virtual FSMBase
{
	virtual void process(const Ea& e)
	{
		throw new std::string("ERR: transition does not exist!");
	}
};

struct TransImpl_Ea_BA : public TransImpl_Ea_Nil
{
	using FSMBase::get_state;
	using FSMBase::set_state;

	virtual void process(const Ea& e)
	{
		if(get_state() == 1)
		{
			B::leave();
			Rule<B, Ea, A>::apply(e);
			A::enter();

			set_state(0);
		}
		else
		{
			TransImpl_Ea_Nil::process(e);
		}
	}
};

struct TransImpl_Ea_AB : public TransImpl_Ea_BA
{
	using FSMBase::get_state;
	using FSMBase::set_state;

	virtual void process(const Ea& e)
	{
		if(get_state() == 0)
		{
			A::leave();
			Rule<A, Ea, B>::apply(e);
			B::enter();

			set_state(1);
		}
		else
		{
			TransImpl_Ea_BA::process(e);
		}
	}
};

struct Tmp1 : public TransImpl_Eb_AA
{
	using TransImpl_Eb_AA::process;
};

struct Tmp2 : public TransImpl_Ea_AB, public Tmp1
{
	using TransImpl_Ea_AB::process;
	using Tmp1::process;
};

struct FSM : public Tmp2
{
	using Tmp2::process;
	FSM() { FSMBase::set_state(0); }
};

typedef FSM MyFSM;

/*
		Transition<A, Ea, B>,
		Transition<A, Eb, A>,
		Transition<B, Ea, A>,
		Transition<B, Eb, A>

	0: A,
	1: B

	((Ea (A B) (B A))
	 (Eb (A A) (B A)))
*/

int main(void)
{
	MyFSM fsm;
	Ea a;
	Eb b;

	fsm.process(a);
	fsm.process(b);
	fsm.process(b);

	return 0;
}

