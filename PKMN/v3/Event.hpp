#ifndef PKMN_BATTLE_EVENT_H
#define PKMN_BATTLE_EVENT_H

#include "common.h"
#include "../UUID.hpp"

namespace pkmn
{
	namespace battle
	{
		enum class Events : BYTE
		{
			SEND_OUT,
			CHOOSE_ATTACK,
			USE_ATTACK,
			MISSED,
			DAMAGE,
			INFO
		};

		class Event
		{

			public:
				Event(dav::UUID id) : trainer_(id) { }
				BYTE ID( ) const { return i_doID(); }
				dav::UUID trainer( ) const { return trainer_; }
			private:
				virtual BYTE i_doID( ) const = 0;
			private:
				dav::UUID trainer_;
		};

		class SendOut_Event : public Event
		{
			public:
				SenOut_Event(dav::UUID trainer, dav::UUID poke) : Event(trainer), poke_(poke) { }
			private:
				BYTE i_doID( ) const { return static_cast<BYTE>(Events::SEND_OUT); }
			private:
				dav::UUID poke_;
		};

		class ChooseAttack_Event : public Event
		{
			private:
				BYTE i_doID( ) const { return static_cast<BYTE>(Events::CHOOSE_ATTACK); }
		};

		class Missed_Event : public Event
		{
			public:
				Missed_Event(dav::UUID trainer, dav::UUID poke) : Event(trainer), poke_(poke) { }
			private:
				BYTE i_doID( ) const { return static_cast<BYTE>(Events::MISSED); }
			private:
				dav::UUID poke_;
		};

		class Info_Event : public Event
		{
			public:
				Info_Event(const std::string& msg) : msg_(msg) { }
			private:
				BYTE i_doID( ) const { return static_cast<BYTE>(Events::INFO); }
		};
	}
}

#endif

