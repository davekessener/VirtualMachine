#include <iostream>

struct Nil
{
	typedef Nil Type;
};

template<typename H, typename T>
struct Cons;
{
	typedef H Head;
	typedef T Tail;
};

template<typename ... T>
struct ConsMaker;

template<typename T, typename ... TT>
struct ConsMaker<T, TT...>
{
	typedef Cons<T, typename ConsMaker<TT...>::type> type;
};

template<>
struct ConsMaker<>
{
	typedef Nil Type;
};

template<typename T1, typename T2>
struct ConsFn
{
	typedef Cons<T1, T2> Type;
};

template<typename ... T>
using MakeList = typename ConsMaker<T...>::Type;

template<typename T>
using Car = typename T::Head;

template<typename T>
using Cdr = typename T::Tail;

template<typename T>
using Caar = Car<Car<T>>;

template<typename T>
using Cadr = Car<Cdr<T>>;

template<typename T>
using Cdar = Cdr<Car<T>>;

template<typename T>
using Cddr = Cdr<Cdr<T>>;

template<int IDX, typename List>
struct GetElem
{
	typedef typename GetElem<IDX - 1, Cdr<List>>::Type Type;
};

template<typename List>
struct GetElem<0, List>
{
	typedef Car<List> Type;
};

template<int IDX, typename List>
using DoGetElem = typename GetElem<IDX, List>::Type;

// ---------------------------------------------------------------------------

template<typename T>
struct InheritLineage : public Car<T>, public InheritLineage<Cdr<T>>
{
};

template<>
struct InheritLineage<Nil>
{
};

template<typename ... T>
using DoInheritLineage = InheritLineage<MakeList<T...>>;

// ---------------------------------------------------------------------------

template<template <class> F, typename T>
struct Apply
{
	typedef Cons<typename F<Car<T>>::Type, typename Apply<F, Cdr<T>>::Type> Type;
}

template<template <class> F>
struct Apply<F, Nil>
{
	typedef Nil Type;
};

// ---------------------------------------------------------------------------

template<typename T1, typename T2>
struct IsSame
{
	static const bool value = false;
};

template<typename T>
struct IsSame<T, T>
{
	static const bool value = true;
};

// ---------------------------------------------------------------------------

template<typename T, T I>
struct Value
{
	static const T value = I;
};

template<int I>
struct Int : Value<int, I>
{
};

template<bool V>
struct Bool : Value<bool, V>
{
};

typedef Bool<true> True;
typedef Bool<false> False;

template<typename T>
struct Identity
{
	typedef T Type;
};

// ---------------------------------------------------------------------------

template<bool Cond, typename T1, typename T2>
struct If;

template<typename T1, typename T2>
struct If<true, T1, T2>
{
	typedef T1 Type;
};

template<typename T1, typename T2>
struct If<false, T1, T2>
{
	typedef T2 Type;
};

template<typename C, typename T1, typename T2>
using DoIf = typename If<C::value, T1, T2>::Type;

// ---------------------------------------------------------------------------

template<typename Map, typename Key>
struct GetValue
{
	typedef DoIf
	<
		IsSame<Caar<Map>, Key>,
		Cdar<Map>,
		GetValue<Cdr<Map>, Key>
	> Type;
};

template<typename Map, typename Key>
using DoGetValue = typename GetValue<Map, Key>::Type;

// ---------------------------------------------------------------------------

template<int IDX, typename List>
struct ListToMapImpl
{
	typedef Cons
	<
		Cons<Int<IDX>, Car<List>>,
		typename ListToMapImpl<IDX + 1, Cdr<List>>::Type
	> Type;
};

template<int IDX>
struct ListToMapImpl<IDX, Nil>
{
	typedef Nil Type;
};

template<typename List>
struct ListToMap
{
	typedef typename ListToMapImpl<0, List>::Type Type;
};

template<typename List>
using DoListToMap = typename ListToMap<List>::Type;

// ---------------------------------------------------------------------------

template<typename Done, typename ToDo>
struct ReverseImpl
{
	typedef typename ReverseImpl
	<
		Cons<Car<ToDo>, Done>,
		Cdr<ToDo>
	>::Type
	Type;
};

template<typename Done>
struct ReverseImpl<Done, Nil>
{
	typedef Done Type;
};

template<typename List>
struct Reverse
{
	typedef typename ReverseImpt<Nil, List>::Type Type;
};

template<typename List>
using DoReverse = typename Reverse<List>::Type;

// ---------------------------------------------------------------------------

template<typename Cell>
struct ReverseCons
{
	typedef Cons<Cdr<Cell>, Car<Cell>> Type;
};

template<typename Cell>
using DoReverseCons = typename ReverseCons<Cell>::Type;

// ---------------------------------------------------------------------------

template<typename T>
struct IsList
{
	static const bool value = false;
};

template<typename T1, typename T2>
struct IsList<Cons<T1, T2>>
{
	static const bool value = true;
};

// ---------------------------------------------------------------------------

template<typename List, typename Appendage>
struct Append
{
	typedef Cons<Car<List>, typename Append<Cdr<List>, Appendage>::Type> Type;
};

template<typename Appendage>
struct Append<Nil, Appendage>
{
	typedef Appendage Type;
};

// ---------------------------------------------------------------------------

template<typename List>
struct Flatten
{
	typedef typename Flatten<Cdr<List>>::Type Type;

	typedef DoIf
	<
		IsList<Car<List>>,
		Append<typename Flatten<Car<List>>::Type, Rest>,
		ConsFn<Car<List>, Rest>
	>
	Type;
};

template<>
struct Flatten<Nil>
{
	typedef Nil Type;
};

template<typename List>
using DoFlatten = typename Flatten<List>::Type;

// ---------------------------------------------------------------------------

template<template <class, class> F, typename T, typename List>
struct Merge
{
	typedef typename Merge<F, typename F<T, Car<List>>::Type, Cdr<List>>::Type Type;
};

template<template <class, class> F, typename T>
struct Merge<F, T, Nil>
{
	typedef T Type;
};

// ---------------------------------------------------------------------------

template<typename T>
struct Not
{
	static const bool value = !T::value;
};

template<typename T1, typename T2>
struct And
{
	static const bool value = T1::value && T2::value;
};

template<typename List>
struct ListAnd
{
	template<typename T1, typename T2>
	struct AndFn
	{
		typedef And<T1, T2> Type;
	};

	typedef typename Merge<AndFn, True, List>::Type Type;
	static const bool value = Type::value;
};

template<typename T1, typename T2>
{
	static const bool value = T1::value || T2::value;
};

template<typename List>
struct ListOr
{
	template<typename T1, typename T2>
	struct OrFn
	{
		typedef Or<T1, T2> Type;
	};

	typedef typename Merge<OrFn, False, List>::Type Type;
	static const bool value = Type::value;
};

// ---------------------------------------------------------------------------

template<typename List, typename T>
struct Contains
{
	static const bool value = IsSame<Car<List>, T>::value || Contains<Cdr<List>, T>::value;
};

template<typename T>
struct Contains<Nil, T>
{
	static const bool value = false;
};

// ---------------------------------------------------------------------------

template<typename Done, typename ToDo>
struct UnifyImpl
{
	typedef typename UnifyImpl
	<
		DoIf
		<
			Contains<Done, Car<ToDo>>,
			Identity<Done>,
			ConsFn<Car<ToDo>, Done>
		>,
		Cdr<ToDo>
	>::Type
	Type;
};

template<typename Done>
struct UnifyImpl<Done, Nil>
{
	typedef DoReverse<Done> Type;
};

template<typename List>
struct Unify
{
	typedef typename UnifyImpl<Nil, List>::Type Type;
};

// ---------------------------------------------------------------------------

template<template <class> F, typename List>
struct Filter
{
	typedef typename Filter<F, Cdr<List>>::Type Rest;

	typedef DoIf
	<
		F<Car<List>>::value,
		ConsFn<Car<List>, Rest>,
		Rest
	>
	Type;
};

template<template <class> F>
struct Filter<F, Nil>
{
	typedef Nil Type;
};

template<template <class> F, typename List>
using DoFilter = typename Filter<F, List>::Type;

// ---------------------------------------------------------------------------

template<typename List>
struct CreateStateList
{
	template<typename T>
	using DoGetStateFromTransition = MakeList<typename T::Origin, typename T::Destination>;

	typedef DoFlatten<DoApply<DoGetStateFromTransition, List>> StateList;
	typedef DoListToMap<DoUnify<StateList>> StateMap;
	typedef DoApply<ReverseCons, StateMap> Type;
};

template<typename List>
using DoCreateStateList = typename CreateStateList<List>::Type;

// ---------------------------------------------------------------------------

template<typename List>
struct CreateTransitionDependencyList
{
	template<typename T>
	using DoGetDependency = MakeList<typename T::Event, Cons<typename T::Origin, typename T::Destination>>;

	typedef DoApply<DoGetDependency, List> RawDependencies;

	template<typename E>
	struct CollectDependencies
	{
		template<typename T>
		using IsCorrectEvent = Same<Car<T>, E>;

		typedef DoApply<Cdr, DoFilter<IsCorrectEvent, RawDependencies>> Dependencies;
		typedef MakeList<E, Dependencies> Type;
	};

	typedef DoUnify<DoApply<Car, RawDependencies>> EventList;
	typedef DoApply<CollectDependencies, EventList> Type;
};

template<typename List>
using DoCreateTransitionDependencyList = typename CreateTransitionDependencyList<List>::Type;

// ---------------------------------------------------------------------------

template<typename T>
struct State
{
	typedef T Data;

	static void enter(Data) { std::cout << "enter base" << std::endl; }
	static void leave(Data) { std::cout << "leave base" << std::endl; }
};

template<class Origin, class Event, class Destination>
struct Rule
{
	static_assert(IsSame<typename Origin::Data, typename Destination::Data>::value, "States need to have identical data-types!");

	typedef typename Origin::Data Data;

	static void apply(const Event&, Data) { std::cout << "empty transition" << std::endl; }
};

template<class O, class E, class D>
struct Transition
{
	typedef O Origin;
	typedef E Event;
	typedef D Destination;
};

struct FSMBase
{
	virtual ~FSMBase() { }
	int get_state() { return state_; }
	void set_state(int state) { state_ = state; }
	int state_;
};

template<typename E, typename L, typename S>
struct TransImpl : public TransImpl<E, Cdr<L>, S>
{
	typedef TransImpl<E, Cdr<L>, S> Super;
	typedef E Event;
	typedef Car<L> Transition;
	typedef S StateList;
	typedef Car<Transition> Origin;
	typedef Cdr<Transition> Destination;
	typedef Rule<Origin, Event, Destination> TransitionFunction;
	typedef typename TransitionFunction::Data Data;
	static const int OriginID = DoGetValue<StateList, Origin>::value;
	static const int DestinationID = DoGetValue<StateList, Destination>::value;
	static const bool IsActualTransition = !Same<Origin, Destination>::value;

	virtual void process(const Event& e, Data d)
	{
		if(get_state() == OriginID)
		{
			if(IsActualTransition)
				Origin::leave(d);

			TransitionFunction::apply(e, d);

			if(IsActualTransition)
				Destination::enter(d);

			set_state(DestinationID);
		}
		else
		{
			Super::process(e, d);
		}
	}
};

template<typename E, typename S>
struct TransImpl<E, Nil, S> : public virtual FSMBase
{
	typedef typename Caar<S>::Data Data;

	virtual void process(const E& e, Data d)
	{
		throw new std::string("ERR: transition does not exist!");
	}
};

template<int ID, typename Inheritance>
struct FSM : public Inheritance
{
	FSM() { set_state(ID); }
};

template<typename I, typename T>
struct FSMMaker
{
	typedef I InitialState;
	typedef DoCreateStateList<T> StateList;
	typedef DoCreateTransitionDependencyList<T> Transitions;
	static const int InitialID = DoGetValue<StateList, InitialState>::value;

	template<typename T>
	using CreateTransitionTree = TransImpl<Car<T>, Cdr<T>, StateList>;

	typedef InheritLineage<DoApply<CreateTransitionTree, Transitions>> Inheritance;

	typedef FSM<InitialID, Inheritance> Type;
};

template<typename I, typename T>
using MakeFSM = typename FSMMaker<I, T>::Type;

// ---------------------------------------------------------------------------

struct A : public State<void>
{
	static void enter(void)
	{
		std::cout << "enter A" << std::endl;
	}
}

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
	static void apply(const Ea& e)
	{
		std::cout << "B -b> A" << std::endl;
	}
};

typedef MakeFSM
<
	A,
	MakeList
	<
		Transition<A, Ea, B>,
		Transition<A, Eb, A>,
		Transition<B, Ea, A>,
		Transition<B, Eb, A>
	>
>
MyFSM;

int main(void)
{
	MyFSM fsm;

	fsm.process(Ea());
	fsm.process(Eb());
	fsm.process(Eb());

	return 0;
}

// ---------------------------------------------------------------------------

//template<int ID, typename Transitions>
//struct Processor;
//
//template<int ID, typename H, typename T>
//struct Processor<ID, Cons<H, T>>
//{
//	typedef typename H::Event Event;
//	typedef typename H::Transition Transition;
//	typedef typename H::StateTable StateTable;
//	static const int OriginID = StateTable::template Lookup<typename Transition::Origin>::value;
//	static const int DestinationID = StateTable::template Lookup<typename Transition::Destination>::value;
//
//	void process(Event& e)
//	{
//	}
//};
//
//
//
//template<int ID, typename T>
//struct Test;
//
//template<int I, typename H, typename T>
//struct Test<I, Cons<H, T>> : public Test<I + 1, T>
//{
//	static const int ID = I;
//	typedef Test<I + 1, T> Super;
//
//	using Super::process;
//
//	void process(const H&)
//	{
//		std::cout << ID << std::endl;
//	}
//};
//
//template<int I>
//struct Test<I, Nil>
//{
//	void process(void) { }
//};
//
//class A { };
//class B { };
//class C { };
//
//typedef Test<0, MakeList<A, B, C>> MyTest;
//
//int main(void)
//{
//	MyTest t;
//
//	t.process(A());
//	t.process(B());
//	t.process(C());
//
//	return 0;
//}
//
//template<class S1, class S2, class T>
//struct Transition;
//
//template
//<
//	typename T,
//	template <typename> S1, 
//	template <typename> S2,
//	template <typename> F
//>
//struct Transition<S1<T>, S2<T>, F<T>>
//{
//	typedef S1<T> OriginState;
//	typedef S2<T> DestinationState;
//	typedef F<T> TransitionFunction;
//	typedef T Data;
//
//	static void transition(Data data)
//	{
//		OriginState::leave(data);
//		TransitionFunction::apply(data);
//		DestinationState::enter(data);
//	}
//};
//
//// ---------------------------------------------------------------------------
//
//template<typename T, typename TT>
//struct FSM
//{
//	typedef T Data;
//	typedef TT Transitions;
//};
//
//template<typename T>
//class Allocator
//{
//	public:
//		static T *allocate( );
//	
//	private:
//		static T *allocated;
//};
//
//template<typename T>
//T *Allocator<T>::allocated = nullptr;
//
//template<typename T>
//T *Allocator<T>::allocate(void)
//{
//	if(allocated)
//		delete allocated;
//
//	return allocated = new T;
//}
//
//// ---------------------------------------------------------------------------
//
//template<typename T, typename E>
//struct LookupWrapper;
//
//template<typename H, typename T, typename E>
//struct LookupWrapper<Cons<H, T>, E>
//{
//	typedef typename LookupWrapper<T, E>::Handler Handler;
//};
//
//template<typename H, typename T>
//struct LookupWrapper<Cons<H, T>, typename H::Event>
//{
//	typedef typename H::Handler Handler;
//};
//
//template<typename Transitions>
//struct Table;
//
//template<typename T, typename TT>
//struct Table<Cons<T, TT>>
//{
//	template<typename E>
//	struct Lookup
//	{
//		typedef typename LookupWrapper<Cons<T, TT>, E>::Handler Handler;
//	};
//};
//
//template<typename State>
//struct Transitioner
//{
//	typedef typename State::BaseState BaseState;
//	typedef typename State::Data Data;
//
//	template<typename E, const BaseState *(State::*F)(Data *) const>
//	struct Transition
//	{
//		typedef E Event;
//
//		struct Handler
//		{
//			static const BaseState *handle(State *state, Data *data)
//			{
//				return (state->*F)(data);
//			}
//		};
//	};
//};
//
//// ---------------------------------------------------------------------------
//
//template<typename InitialState>
//class FSM
//{
//	private:
//		typedef typename InitialState::Data Data;
//		typedef typename InitialState::BaseState BaseState;
//
//	public:
//		FSM( ) 
//			: state_(InitialState::instance())
//			, data_(Allocator<Data>::allocate()) 
//			{ }
//		~FSM( ) { delete data_; }
//		template<typename E>
//			void consume(const E&);
//	private:
//		const BaseState *state_;
//		Data *data_;
//};
//
//template<typename T>
//template<typename E>
//void FSM<T>::consume(const E& e)
//{
//	state_ = state_->consume(e, data_);
//}
//
//// ===========================================================================
//
//#define MAKELOOKUPFOR(state,...) \
//template<typename E> struct Lookup { \
//struct LookupGenerator : public Transitioner<state> { \
//typedef Table<MakeList<__VA_ARGS__>> StateTable; \
//}; \
//typedef typename LookupGenerator::StateTable StateTable; \
//typedef typename StateTable::template Lookup<E>::Handler Handler; \
//}; \
//static BaseState *instance() { static state inst; return &inst; } \
//template<typename E> \
//const BaseState *consume(const E&, Data *data) const { \
//	typedef typename Lookup<E>::Handler Handler; \
//	return Handler::handle(this, data); \
//}
//
//template<typename T>
//struct CommonState
//{
//	typedef CommonState<T> BaseState;
//	typedef T Data;
//
//	template<typename E>
//	const BaseState *consume(const E&, Data *) const
//	{
//		throw new std::string("FUCK");
//	}
//};
//
//struct EventA { };
//struct EventB { };
//
//struct MyState : public CommonState<Nil>
//{
//};
//
//struct StateB : public MyState
//{
//	const BaseState *reactA(Data *data) const
//	{
//		std::cout << "B:A" << std::endl;
//
//		return this;
//	}
//
//	const BaseState *reactB(Data *data) const
//	{
//		std::cout << "B:B" << std::endl;
//
//		return this;
//	}
//
//	MAKELOOKUPFOR(StateB,
//		Transition<EventA, &StateB::reactA>,
//		Transition<EventB, &StateB::reactB>
//	)
//};
//
//struct StateA : public MyState
//{
//	const BaseState *reactA(Data *data) const
//	{
//		std::cout << "A:A" << std::endl;
//
//		return StateB::instance();
//	}
//
//	const BaseState *reactB(Data *data) const
//	{
//		std::cout << "A:B" << std::endl;
//
//		return this;
//	}
//
//	MAKELOOKUPFOR(StateA,
//		Transition<EventA, &StateA::reactA>,
//		Transition<EventB, &StateA::reactB>
//	)
//};
//
//typedef FSM<StateA> MyFSM;
//
//// =========================================================================== 
//
//int main(void)
//{
//	MyFSM fsm;
//
//	EventA a;
//	EventB b;
//
//	fsm.consume(b);
//	fsm.consume(a);
//	fsm.consume(b);
//	fsm.consume(a);
//
//	return 0;
//}
//
