#ifndef PKMN_BATTLE_EVENT_H
#define PKMN_BATTLE_EVENT_H

#include "common.h"
#include "../UUID.hpp"
#include "../Pokemon.h"

namespace pkmn
{
	namespace battle
	{
		enum class Events : BYTE
		{
			DEMAND,
			SEND_OUT,
			USE_ATTACK,
			MISSED,
			DAMAGE,
			STAT,
			INFO
		};

		enum class Demands : BYTE
		{
			CHOOSE_POKEMON,
			CHOOSE_ACTION
		};

		enum class Info : BYTE
		{
			CHALLENGED,
			FAILED,
			SUPER_EFFECTIVE,
			NOT_EFFECTIVE
		};

		class Event
		{
			public:
			typedef std::shared_ptr<Event> Event_ptr;

			public:
				Event(dav::UUID id) : trainer_(id) { }
				BYTE ID( ) const { return i_doID(); }
				dav::UUID trainer( ) const { return trainer_; }
				void writeToNBT(nbt::TAG_Compound_ptr_t tag) const
				{
					i_writeToNBT(tag);
					tag->setString("Trainer-UUID", trainer_.toString());
				}
				void readFromNBT(nbt::TAG_Compound_ptr_t tag)
				{
					i_readFromNBT(tag);
					trainer_ = dav::UUID(tag->getString("Trainer-UUID"));
				}
				template<typename T, typename ... TT>
				static Event_ptr Make(TT&& ... tt)
				{
					return Event_ptr(new T(tt...));
				}
			private:
				virtual BYTE i_doID( ) const = 0;
				virtual void i_writeToNBT(nbt::TAG_Compound_ptr_t) const { }
				virtual void i_readFromNBT(nbt::TAG_Compound_ptr_t nbt) { }
			private:
				dav::UUID trainer_;
		};

		typedef Event::Event_ptr Event_ptr;

		class SendOut_Event : public Event
		{
			public:
				SendOut_Event(dav::UUID trainer, Pokemon_ptr poke) : Event(trainer), poke_(poke) { }
			private:
				BYTE i_doID( ) const { return static_cast<BYTE>(Events::SEND_OUT); }
				void i_writeToNBT(nbt::TAG_Compound_ptr_t tag) const
				{
					nbt::TAG_Compound_ptr_t p = nbt::Make<nbt::TAG_Compound>();
					poke_->writeToNBT(p);
					tag->setCompoundTag("Pokemon", p);
				}
				void i_readFromNBT(nbt::TAG_Compound_ptr_t tag)
				{
					poke_.reset(new Pokemon);
					poke_->readFromNBT(tag);
				}
			private:
				Pokemon_ptr poke_;
		};

		class UseMove_Event : public Event
		{
			public:
				UseMove_Event(dav::UUID t, dav::UUID p, const std::string& a) : Event(t), poke_(p), move_(a) { }
				const dav::UUID& pokemon( ) const { return poke_; }
				const std::string& move( ) const { return move_; }
			private:
				BYTE i_doID( ) const { return static_cast<BYTE>(Events::USE_ATTACK); }
				void i_writeToNBT(nbt::TAG_Compound_ptr_t tag) const
				{
					tag->setString("Pokemon-UUID", poke_.toString());
					tag->setString("Move", move_);
				}
				void i_readFromNBT(nbt::TAG_Compound_ptr_t tag)
				{
					poke_ = dav::UUID(tag->getString("Pokemon-UUID"));
					move_ = tag->getString("Move");
				}
			private:
				dav::UUID poke_;
				std::string move_;
		};

		class Demand_Event : public Event
		{
			public:
				Demand_Event(dav::UUID t, Demands d) : Event(t), demand_(d) { }
				Demands demand( ) const { return demand_; }
			private:
				BYTE i_doID( ) const { return static_cast<BYTE>(Events::DEMAND); }
				void i_writeToNBT(nbt::TAG_Compound_ptr_t tag) const
					{ tag->setByte("Demand", static_cast<BYTE>(demand_)); }
				void i_readFromNBT(nbt::TAG_Compound_ptr_t tag)
					{ demand_ = static_cast<Demands>((BYTE)tag->getByte("Demand")); }
			private:
				Demands demand_;
		};

		class Missed_Event : public Event
		{
			public:
				Missed_Event(dav::UUID trainer, dav::UUID poke) : Event(trainer), poke_(poke) { }
				dav::UUID pokemon( ) const { return poke_; }
			private:
				BYTE i_doID( ) const { return static_cast<BYTE>(Events::MISSED); }
				void i_writeToNBT(nbt::TAG_Compound_ptr_t tag) const
					{ tag->setString("Pokemon-UUID", poke_.toString()); }
				void i_readFromNBT(nbt::TAG_Compound_ptr_t tag)
					{ poke_ = dav::UUID(tag->getString("Pokemon-UUID")); }
			private:
				dav::UUID poke_;
		};

		class Info_Event : public Event
		{
			public:
				Info_Event(Info msg) : Event(dav::UUID::Nil()), msg_(msg) { }
				Info message( ) const { return msg_; }
			private:
				BYTE i_doID( ) const { return static_cast<BYTE>(Events::INFO); }
				void i_writeToNBT(nbt::TAG_Compound_ptr_t tag) const { tag->setByte("Message", static_cast<BYTE>(msg_)); }
				void i_readFromNBT(nbt::TAG_Compound_ptr_t tag) { msg_ = static_cast<Info>((BYTE)tag->getByte("Message")); }
			private:
				Info msg_;
		};

		class Stat_Event : public Event
		{
			public:
				Stat_Event(dav::UUID trainer, dav::UUID pokemon, uint stat, int stages)
					: Event(trainer), poke_(pokemon), stat_(stat), stages_(stages)
				{
				}
				const dav::UUID& pokemon( ) const { return poke_; }
				uint stat( ) const { return stat_; }
				int stages( ) const { return stages_; }
			private:
				BYTE i_doID( ) const { return static_cast<BYTE>(Events::STAT); }
				void i_writeToNBT(nbt::TAG_Compound_ptr_t tag) const
				{
					tag->setInt("Stat", stat_);
					tag->setInt("Stages", stages_);
				}
				void i_readFromNBT(nbt::TAG_Compound_ptr_t tag)
				{
					stat_ = tag->getInt("Stat");
					stages_ = tag->getInt("Stages");
				}
			private:
				dav::UUID poke_;
				uint stat_;
				int stages_;
		};
	}
}

#endif

