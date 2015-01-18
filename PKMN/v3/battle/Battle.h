#ifndef PKMN_BATTLE_BATTLE_H
#define PKMN_BATTLE_BATTLE_H

#include <map>
#include <deque>
#include "common.h"
#include "../UUID.hpp"
#include "../Pokemon.h"
#include "Event.hpp"
#include "Action.hpp"
#include "Pokemon.h"

namespace pkmn
{
	namespace battle
	{
		class Battle
		{
			struct Trainer
			{
				dav::UUID id;
				Pokemon poke;
				Pokemon_ptr data;
				Action_ptr action;
			};

			public:
				void registerTrainers(dav::UUID, Pokemon_ptr, dav::UUID, Pokemon_ptr);
				void switchPokemon(dav::UUID, Pokemon_ptr);
				void useMove(dav::UUID, dav::UUID, const std::string&);
				bool hasEvents( ) const { return !events_.empty(); }
				Event_ptr getEvent( );
			private:
				bool hasTrainers( ) const;
				bool havePokemon( ) const;
				void checkTrainer(dav::UUID) const;
				dav::UUID otherTrainer(dav::UUID) const;
				void queueEvent(Event_ptr p) { events_.push_back(p); }
				void addAction(Action_ptr);
				void act(Action_ptr);
				void actAttack(const Attack_Action&);
				void actSwitch(const Switch_Action&);
			private:
				std::map<dav::UUID, Trainer> trainers_;
				std::deque<Event_ptr> events_;
		};
	}
}

#endif

