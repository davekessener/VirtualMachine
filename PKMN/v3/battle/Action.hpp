#ifndef PKMN_BATTLE_ACTION_H
#define PKMN_BATTLE_ACTION_H

#include <memory>
#include "common.h"
#include "../UUID.hpp"

namespace pkmn
{
	namespace battle
	{
		enum class Actions : BYTE
		{
			ATTACK,
			SWITCH,
			ITEM,
			FLEE
		};

		class Action : public std::enable_shared_from_this<Action>
		{
			public:
			typedef std::shared_ptr<Action> Action_ptr;

			public:
				Action(dav::UUID t) : trainer_(t) { }
				const dav::UUID& trainer( ) const { return trainer_; }
				Actions action( ) const { return i_doAction(); }
				int priority( ) const { return i_priority(); }
				template<typename T>
				std::shared_ptr<T> cast( ) { return std::dynamic_pointer_cast<T>(shared_from_this()); }
				template<typename T, typename ... TT>
				static Action_ptr Make(TT&& ... tt)
				{
					return Action_ptr(new T(tt...));
				}
			private:
				virtual Actions i_doAction( ) const = 0;
				virtual int i_priority( ) const { return 0; }
			private:
				dav::UUID trainer_;
		};

		typedef Action::Action_ptr Action_ptr;

		class Attack_Action : public Action
		{
			public:
				Attack_Action(dav::UUID t, dav::UUID p, uint m, int pr) : Action(t), poke_(p), move_(m), prior_(pr) { }
				const dav::UUID& pokemon( ) const { return poke_; }
				uint move( ) const { return move_; }
			private:
				Actions i_doAction( ) const { return Actions::ATTACK; }
				int i_priority( ) const { return prior_; }
			private:
				dav::UUID poke_;
				uint move_;
				int prior_;
		};

		class Switch_Action : public Action
		{
			public:
				Switch_Action(dav::UUID t, Pokemon_ptr p) : Action(t), poke_(p) { }
				Pokemon_ptr pokemon( ) const { return poke_; }
			private:
				Actions i_doAction( ) const { return Actions::SWITCH; }
				int i_priority( ) const { return 6; }
			private:
				Pokemon_ptr poke_;
		};
	}
}

#endif

