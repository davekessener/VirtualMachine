#include <cassert>
#include "Battle.h"

namespace pkmn { namespace battle {

void Battle::registerTrainers(dav::UUID t1, Pokemon_ptr p1, dav::UUID t2, Pokemon_ptr p2)
{
	if(hasTrainers()) throw std::string("ERR: Has already trianers!");
	if(!static_cast<bool>(t1)) throw std::string("ERR: cannot register trainer with id NIL!");
	if(t1 == t2) throw std::string("ERR: Cannot register same trainer twice!");

	trainers_[t1].id = t1;
	trainers_[t1].poke = Pokemon::Create(p1);
	trainers_[t1].data = p1;
	trainers_[t2].id = t2;
	trainers_[t2].poke = Pokemon::Create(p2);
	trainers_[t2].data = p2;

	std::cout << "Trainer registered: " << t1.toString() << std::endl;
	std::cout << "Trainer registered: " << t2.toString() << std::endl;

//	queueEvent(Event::Make<Info_Event>(t1, Info::CHALLENGED));
//	queueEvent(Event::Make<Info_Event>(t2, Info::CHALLENGED));
	queueEvent(Event::Make<SendOut_Event>(t1, p1));
	queueEvent(Event::Make<SendOut_Event>(t2, p2));
	queueEvent(Event::Make<Demand_Event>(t1, Demands::CHOOSE_ACTION));
	queueEvent(Event::Make<Demand_Event>(t2, Demands::CHOOSE_ACTION));
}

void Battle::switchPokemon(dav::UUID t, Pokemon_ptr p)
{
	checkTrainer(t);

	if(!trainers_[t].poke)
	{
		trainers_[t].poke = Pokemon::Create(p);
		trainers_[t].data = p;

		queueEvent(Event::Make<SendOut_Event>(t, p));
		queueEvent(Event::Make<Demand_Event>(t, Demands::CHOOSE_ACTION));
	}
	else
	{
		addAction(Action::Make<Switch_Action>(t, p));
	}
}

void Battle::useMove(dav::UUID t, dav::UUID p, uint idx)
{
	checkTrainer(t);

	if(p != trainers_[t].data->id) throw std::string("ERR: pokemon doesn't exist!");

	Pokemon &poke = trainers_[t].poke;

	if(!poke || !poke.health) throw std::string("ERR: No pokemon to attack!");

	if(!poke.moves[idx].move) throw std::string("ERR: Invalid move.");

	if(!poke.moves[idx].pp) throw std::string("ERR: No PP left!");

	addAction(Action::Make<Attack_Action>(t, p, idx, poke.moves[idx].move->priority));
}

Event_ptr Battle::getEvent(void)
{
	if(events_.empty()) throw std::string("ERR: No event queued!");

	Event_ptr p = events_.front();
	
	events_.pop_front();

	return p;
}

// # ===========================================================================

void Battle::addAction(Action_ptr a)
{
	dav::UUID t = a->trainer();

	checkTrainer(t);

	if(trainers_[t].action) throw std::string("ERR: trainer already has an action!");

	trainers_[t].action = a;

	Action_ptr o;

	for(auto& p : trainers_)
	{
		if(!static_cast<bool>(p.second.action)) return;
		if(p.second.id != t) o = p.second.action;
	}

	if(a->priority() > o->priority())
	{
		act(a);
		act(o);
	}
	else if(a->priority() < o->priority())
	{
		act(o);
		act(a);
	}
	else
	{
		if(trainers_[a->trainer()].poke.Spe() < trainers_[o->trainer()].poke.Spe())
		{
			act(o);
			act(a);
		}
		else
		{
			act(a);
			act(o);
		}
	}

	for(auto& p : trainers_)
	{
		p.second.action.reset();
	}

	for(auto& p : trainers_)
	{
		if(!p.second.poke.health) queueEvent(Event::Make<Fainted_Event>(p.second.id, p.second.data->id));
	}

	for(auto& p : trainers_)
	{
		queueEvent(Event::Make<Demand_Event>(p.second.id,
			p.second.poke.health ? Demands::CHOOSE_ACTION : Demands::CHOOSE_POKEMON));
	}
}

void Battle::act(Action_ptr a)
{
	switch(a->action())
	{
		case Actions::ATTACK:
			actAttack(*a->cast<Attack_Action>());
			break;
		case Actions::ITEM:
//			actItem(*a->cast<Item_Action>());
			break;
		case Actions::SWITCH:
			actSwitch(*a->cast<Switch_Action>());
			break;
		case Actions::FLEE:
//			actFlee(*a->cast<Flee_action>());
			break;
	}
}

void Battle::actAttack(const Attack_Action& a)
{
	dav::UUID t1(a.trainer()), t2(otherTrainer(a.trainer()));
	Pokemon &user = trainers_[t1].poke;
	Pokemon &target = trainers_[t2].poke;

	if(user.health == 0) return;

	assert(trainers_[t1].data->id == a.pokemon() && user.moves[a.move()].move); 

	auto &move = *user.moves[a.move()].move;

	//if move has effect TODO

	queueEvent(Event::Make<UseMove_Event>(t1, trainers_[t1].data->id, a.move()));

	if(move.accuracy > 0.0 && !dav::UUID::Bool(move.accuracy * user.Accuracy() / target.Evasion()))
	{
		queueEvent(Event::Make<Missed_Event>(t1, a.pokemon()));
		return;
	}

	if(move.category != Category::STATUS)
	{
		float type = Types::Modifier(move.type, target.types[0]);
		if(!target.types[1].empty()) type *= Types::Modifier(move.type, target.types[1]);

		if(type == 0.0)
		{
			queueEvent(Event::Make<Info_Event>(Info::FAILED));
			return;
		}

		float stab = move.type == user.types[0] || move.type == user.types[1] ? 1.5 : 1.0;
		float crit = dav::UUID::Bool(0.0625) ? 1.5 : 1.0;
		float r = 0.85 + 0.15 * (dav::UUID::rand<uint>() / (double)((uint) -1));

		float mod = type * stab * crit * r;
		uint atk = move.category == Category::PHYSICAL ? user.Atk() : user.SpA();
		uint def = move.category == Category::PHYSICAL ? target.Def() : target.SpD();

		uint damage = ((2 * user.level + 10) * atk * move.power / 250.0 / def + 2) * mod + 0.5;

		if(damage > target.health) damage = target.health;

		target.health -= damage;
		queueEvent(Event::Make<Damage_Event>(t2, trainers_[t2].data->id, damage));

		if(crit > 1.0) queueEvent(Event::Make<Info_Event>(Info::CRITICAL_HIT));
		if(type > 1.0) queueEvent(Event::Make<Info_Event>(Info::SUPER_EFFECTIVE));
		else if(type < 1.0) queueEvent(Event::Make<Info_Event>(Info::NOT_EFFECTIVE));
	}

	for(auto &sc : move.stat_changes)
	{
		if(sc.chance < 1.0 && !dav::UUID::Bool(sc.chance)) continue;
		bool self(sc.target == Target::SELF);
		int ac = (self ? user : target).applyStat(sc.stat, sc.stages);
		queueEvent(Event::Make<Stat_Event>(self ? t1 : t2, trainers_[self ? t1 : t2].data->id, sc.stat, sc.stages, ac));
	}

	for(auto &sc : move.status_changes)
	{
	}
}

void Battle::actSwitch(const Switch_Action& a)
{
}

// # ---------------------------------------------------------------------------

bool Battle::hasTrainers(void) const
{
	return trainers_.size() == 2;
}

bool Battle::havePokemon(void) const
{
	if(!hasTrainers()) throw std::string("ERR: No trainers to check pokemon!");

	for(auto& p : trainers_)
	{
		if(!p.second.poke) return false;
		if(!p.second.poke.health) /*return false;*/ throw std::string("ERR: Fainted pokemon!");
	}

	return true;
}

void Battle::checkTrainer(dav::UUID t) const
{
	if(!hasTrainers()) throw std::string("ERR: Not initialized!");
	if(!trainers_.count(t)) throw std::string("ERR: Unknown trainer(" + t.toString() + ")!");
}

dav::UUID Battle::otherTrainer(dav::UUID t) const
{
	checkTrainer(t);

	for(auto& p : trainers_)
	{
		if(p.second.id != t) return p.second.id;
	}

	throw std::string("ERR: no other trainer! WTF");
}

}}

