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
			FAINTED,
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
			CRITICAL_HIT,
			SUPER_EFFECTIVE,
			NOT_EFFECTIVE
		};

		class Event
		{
			public:
			typedef std::shared_ptr<Event> Event_ptr;

			public:
				Event(dav::UUID id) : trainer_(id) { }
				Events ID( ) const { return i_doID(); }
				dav::UUID trainer( ) const { return trainer_; }
				void writeToNBT(nbt::TAG_Compound_ptr_t tag) const
				{
					i_writeToNBT(tag);
					tag->setString("Trainer-UUID", trainer_.toString());
					tag->setByte("ID", static_cast<BYTE>(ID()));
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
				virtual Events i_doID( ) const = 0;
				virtual void i_writeToNBT(nbt::TAG_Compound_ptr_t) const { }
				virtual void i_readFromNBT(nbt::TAG_Compound_ptr_t nbt) { }
			private:
				dav::UUID trainer_;
		};

		typedef Event::Event_ptr Event_ptr;

		class SendOut_Event : public Event
		{
			public:
				SendOut_Event(dav::UUID trainer, Pokemon_ptr poke) : Event(trainer), poke_(poke)
				{
					nbt::TAG_Compound_ptr_t tag = nbt::Make<nbt::TAG_Compound>();
					i_writeToNBT(tag);
					i_readFromNBT(tag);
				}
				Pokemon_ptr pokemon( ) const { return poke_; }
			private:
				Events i_doID( ) const { return Events::SEND_OUT; }
				void i_writeToNBT(nbt::TAG_Compound_ptr_t tag) const
				{
					nbt::TAG_Compound_ptr_t p = nbt::Make<nbt::TAG_Compound>();
					poke_->writeToNBT(p);
					tag->setCompoundTag("Pokemon", p);
				}
				void i_readFromNBT(nbt::TAG_Compound_ptr_t tag)
				{
					poke_.reset(new Pokemon);
					poke_->readFromNBT(tag->getCompoundTag("Pokemon"));
				}
			private:
				Pokemon_ptr poke_;
		};

		class UseMove_Event : public Event
		{
			public:
				UseMove_Event(dav::UUID t, dav::UUID p, uint a) : Event(t), poke_(p), move_(a) { }
				const dav::UUID& pokemon( ) const { return poke_; }
				uint move( ) const { return move_; }
			private:
				Events i_doID( ) const { return Events::USE_ATTACK; }
				void i_writeToNBT(nbt::TAG_Compound_ptr_t tag) const
				{
					tag->setString("Pokemon-UUID", poke_.toString());
					tag->setInt("Move", move_);
				}
				void i_readFromNBT(nbt::TAG_Compound_ptr_t tag)
				{
					poke_ = dav::UUID(tag->getString("Pokemon-UUID"));
					move_ = tag->getInt("Move");
				}
			private:
				dav::UUID poke_;
				uint move_;
		};

		class Demand_Event : public Event
		{
			public:
				Demand_Event(dav::UUID t, Demands d) : Event(t), demand_(d) { }
				Demands demand( ) const { return demand_; }
			private:
				Events i_doID( ) const { return Events::DEMAND; }
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
				Events i_doID( ) const { return Events::MISSED; }
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
				Info info( ) const { return msg_; }
			private:
				Events i_doID( ) const { return Events::INFO; }
				void i_writeToNBT(nbt::TAG_Compound_ptr_t tag) const { tag->setByte("Message", static_cast<BYTE>(msg_)); }
				void i_readFromNBT(nbt::TAG_Compound_ptr_t tag) { msg_ = static_cast<Info>((BYTE)tag->getByte("Message")); }
			private:
				Info msg_;
		};

		class Damage_Event : public Event
		{
			public:
				Damage_Event(dav::UUID t, dav::UUID p, uint d) : Event(t), poke_(p), damage_(d) { }
				uint damage( ) const { return damage_; }
			private:
				Events i_doID( ) const { return Events::DAMAGE; }
				void i_writeToNBT(nbt::TAG_Compound_ptr_t tag) const { tag->setString("Pokemon-UUID", poke_.toString()); tag->setInt("Damage", damage_); }
				void i_readFromNBT(nbt::TAG_Compound_ptr_t tag) { poke_ = dav::UUID(tag->getString("Pokemon-UUID")); damage_ = tag->getInt("Damage"); }
			private:
				dav::UUID poke_;
				uint damage_;
		};

		class Stat_Event : public Event
		{
			public:
				Stat_Event(dav::UUID trainer, dav::UUID pokemon, uint stat, int stages, int actual)
					: Event(trainer), poke_(pokemon), stat_(stat), stages_(stages), actual_(actual)
				{
				}
				const dav::UUID& pokemon( ) const { return poke_; }
				uint stat( ) const { return stat_; }
				int stages( ) const { return stages_; }
				int actualStages( ) const { return actual_; }
			private:
				Events i_doID( ) const { return Events::STAT; }
				void i_writeToNBT(nbt::TAG_Compound_ptr_t tag) const
				{
					tag->setInt("Stat", stat_);
					tag->setInt("Stages", stages_);
					tag->setInt("ActualStages", actual_);
				}
				void i_readFromNBT(nbt::TAG_Compound_ptr_t tag)
				{
					stat_ = tag->getInt("Stat");
					stages_ = tag->getInt("Stages");
					actual_ = tag->getInt("ActualStages");
				}
			private:
				dav::UUID poke_;
				uint stat_;
				int stages_, actual_;
		};

		class Fainted_Event : public Event
		{
			public:
				Fainted_Event(dav::UUID t, dav::UUID p) : Event(t), poke_(p) { }
				const dav::UUID& pokemon( ) const { return poke_; }
			private:
				Events i_doID( ) const { return Events::FAINTED; }
				void i_writeToNBT(nbt::TAG_Compound_ptr_t tag) const { tag->setString("Pokemon-UUID", poke_.toString()); }
				void i_readFromNBT(nbt::TAG_Compound_ptr_t tag) { poke_ = dav::UUID(tag->getString("Pokemon-UUID")); }
			private:
				dav::UUID poke_;
		};
	}
}

#endif

