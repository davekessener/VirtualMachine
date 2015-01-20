#include <iostream>
#include "common.h"
#include "battle/Battle.h"
#include "battle/Event.hpp"
#include <nbt/NBT.h>
#include <aux>

namespace pkmn
{
	using battle::Event;
	using battle::Event_ptr;
	using battle::Demand_Event;
	using battle::SendOut_Event;
	using battle::UseMove_Event;
	using battle::Missed_Event;
	using battle::Damage_Event;
	using battle::Stat_Event;
	using battle::Fainted_Event;
	using battle::Info_Event;
	using battle::Info;
	using battle::Events;
	using battle::Demands;

	class Trainer
	{
		public:
			Trainer(battle::Battle *b, std::ostream *os = nullptr) : battle_(*b), os_(os) { }
			const dav::UUID& ID( ) const { return id_; }
			Pokemon_ptr pokemon( ) const { return poke_; }
			void generate(const std::string& s, uint l)
				{ if(poke_) throw std::string("ERR: has already pokemon!"); poke_ = Pokemon::Generate(s, l); }
			void handleEvent(Event_ptr);
		private:
			virtual void handleDemand(const Demand_Event&);
			virtual void handleSendOut(const SendOut_Event&);
			virtual void handleUseMove(const UseMove_Event&);
			virtual void handleMissed(const Missed_Event&);
			virtual void handleDamage(const Damage_Event&);
			virtual void handleStat(const Stat_Event&);
			virtual void handleFainted(const Fainted_Event&);
			virtual void handleInfo(const Info_Event&);
		private:
			void printStatus( ) const;
			std::string readCmd( ) const;
			template<typename ... TT>
			void print(TT&& ... tt) const { if(os_) (*os_) << dav::aux::stringify(tt...) << std::flush; }
			template<typename ... TT>
			void println(TT&& ... tt) const { print(tt..., '\n'); }
			std::string getName(dav::UUID id) const { return id == id_ ? poke_->Name() : "Foe " + opp_->Name(); }
		protected:
			dav::UUID id_;
			battle::Battle &battle_;
			Pokemon_ptr poke_, opp_;
		private:
			std::ostream *os_;
	};

	class AITrainer : public Trainer
	{
		public:
			AITrainer(battle::Battle *b) : Trainer(b) { }
		private:
			void handleDemand(const Demand_Event&);
	};

	class Battle
	{
		public:
			void run( );
		private:
			battle::Battle battle_;
			nbt::TAG_Compound_ptr_t data_;
			std::shared_ptr<Trainer> trainer_[2];
	};

	void Battle::run(void)
	{
		data_ = nbt::readFile("pokemon.nbt");

		SpeciesManager::Load(data_->getTagList("Species"));
		Moves::Load(data_->getTagList("Moves"));

		trainer_[0].reset(new Trainer(&battle_, &std::cout));
		trainer_[1].reset(new AITrainer(&battle_));

		Trainer &me(*trainer_[0]), &opp(*trainer_[1]);

		me.generate("charizard", 100);
		opp.generate("charizard", 100);

		battle_.registerTrainers(me.ID(), me.pokemon(), opp.ID(), opp.pokemon());

		while(battle_.hasEvents())
		{
			Event_ptr e = battle_.getEvent();
			opp.handleEvent(e);
			me.handleEvent(e);
		}
	}

	void Trainer::handleEvent(Event_ptr e)
	{
		switch(e->ID())
		{
			case Events::DEMAND:
				handleDemand(*std::dynamic_pointer_cast<Demand_Event>(e));
				break;
			case Events::SEND_OUT:
				handleSendOut(*std::dynamic_pointer_cast<SendOut_Event>(e));
				break;
			case Events::USE_ATTACK:
				handleUseMove(*std::dynamic_pointer_cast<UseMove_Event>(e));
				break;
			case Events::MISSED:
				handleMissed(*std::dynamic_pointer_cast<Missed_Event>(e));
				break;
			case Events::DAMAGE:
				handleDamage(*std::dynamic_pointer_cast<Damage_Event>(e));
				break;
			case Events::STAT:
				handleStat(*std::dynamic_pointer_cast<Stat_Event>(e));
				break;
			case Events::FAINTED:
				handleFainted(*std::dynamic_pointer_cast<Fainted_Event>(e));
				break;
			case Events::INFO:
				handleInfo(*std::dynamic_pointer_cast<Info_Event>(e));
				break;
		}
	}

	void AITrainer::handleDemand(const Demand_Event& e)
	{
		if(e.demand() == Demands::CHOOSE_POKEMON) exit(0);
		if(e.trainer() != id_) return;
		battle_.useMove(id_, poke_->id, 0);
	}

	void Trainer::handleDemand(const Demand_Event& e)
	{
		if(e.demand() == Demands::CHOOSE_POKEMON) exit(0);

		if(e.trainer() != id_) return;

		while(true)
		{
			printStatus();
			std::string s = readCmd();

			uint m = 0;
			if(s == "1") m = 1;
			else if(s == "2") m = 2;
			else if(s == "3") m = 3;
			else if(s == "4") m = 4;
			else if(s == "flee")
			{
				println("You escaped successfully!");
				exit(0);
			}

			if(!m || poke_->moves[m - 1].id.empty() || !poke_->moves[m - 1].pp)
			{
				println("Invalid input! Try again.\n");
			}
			else
			{
				battle_.useMove(id_, poke_->id, m - 1);
				break;
			}
		}
	}

	void Trainer::handleSendOut(const SendOut_Event& e)
	{
		if(e.trainer() == id_)
		{
			println("Go, ", poke_->Name(), "!");
		}
		else
		{
			opp_ = e.pokemon();
			println("Opponent sent out ", opp_->Name(), "!");
		}
	}

	void Trainer::handleUseMove(const UseMove_Event& e)
	{
		bool isme = e.trainer() == id_;
		Pokemon_ptr p = isme ? poke_ : opp_;
		println((isme ? "" : "Foe "), p->Name(), " used ", Moves::Get(p->moves[e.move()].id).name, ".");
		--(p->moves[e.move()].pp);
	}

	void Trainer::handleMissed(const Missed_Event& e)
	{
		println((e.trainer() == id_ ? poke_->Name() : "Foe " + opp_->Name()), "'s attack missed!");
	}

	void Trainer::handleDamage(const Damage_Event& e)
	{
		Pokemon_ptr p = e.trainer() == id_ ? poke_ : opp_;
		p->health -= e.damage();
		println(getName(e.trainer()), " suffered ", e.damage(), "(", (uint)(1000 * e.damage() / (double)p->stats[0]) / 10.0, "%) damage!");
	}

	void Trainer::handleStat(const Stat_Event& e)
	{
		std::string n = getName(e.trainer());
		auto sname = [](uint i) -> std::string
		{
			if(i == 0) return "HP";
			else if(i == 1) return "Attack";
			else if(i == 2) return "Defense";
			else if(i == 3) return "Sp.Attack";
			else if(i == 4) return "Sp.Defense";
			else if(i == 5) return "Speed";
			else if(i == 6) return "Accuracy";
			else if(i == 7) return "Evasion";
			else throw std::string("ERR: Invalid stat!");
		};

		if(!e.actualStages())
		{
			println(n, "'s ", sname(e.stat()), " can't ", e.stages() < 0 ? "fall" : "increase", " further.");
		}
		else
		{
			println(n, "'s ", sname(e.stat()), e.actualStages() * e.actualStages() >= 4 ? " sharply " : " ", e.stages() < 0 ? "fell!" : "increased!");
		}
	}

	void Trainer::handleFainted(const Fainted_Event& e)
	{
		if(e.trainer() == id_)
		{
			println(poke_->Name(), " fainted. You lost.");
		}
		else
		{
			println("Foe ", opp_->Name(), " fainted. You won!");
		}
	}

	void Trainer::handleInfo(const Info_Event& e)
	{
		switch(e.info())
		{
			case Info::CHALLENGED:
				break;
			case Info::FAILED:
				println("But it failed ...");
				break;
			case Info::CRITICAL_HIT:
				println("Critical hit!");
				break;
			case Info::SUPER_EFFECTIVE:
				println("It's super effective!");
				break;
			case Info::NOT_EFFECTIVE:
				println("It's not very effective ...");
				break;
		}
	}

	void Trainer::printStatus(void) const
	{
		println("\n#=========================================");
		println("# Opp: ", opp_->Name(), " ", (int)(1000 * opp_->health / (double)opp_->stats[0]) / 10.0, "%");
		println("# You: ", poke_->Name(), " ", poke_->health, "/", poke_->stats[0], "(", (int)(1000 * poke_->health / (double)poke_->stats[0]) / 10.0, "%)");
		println("## HP  ", poke_->stats[0], "\tSpe ", poke_->stats[5]);
		println("## Atk ", poke_->stats[1], "\tDef ", poke_->stats[2]);
		println("## SpA ", poke_->stats[3], "\tSpD ", poke_->stats[4]);
		for(int i = 0 ; i < 4 ; ++i)
		{
			if(poke_->moves[i].id.empty()) break;
			auto &pm(poke_->moves[i]);
			auto &m(Moves::Get(pm.id));
			print("# ", i + 1, " - ", m.name);
			for(size_t j = m.name.size() ; j < 20 ; ++j) print(' ');
			println(pm.pp, "/", m.pp * (5 + pm.pp_ups) / 5);
		}
	}

	std::string Trainer::readCmd(void) const
	{
		std::string cmd("");

		while(cmd.empty())
		{
			print("\n> ");
			std::getline(std::cin, cmd);
			if(cmd.empty()) continue;
			const char *b, *e;
			b = cmd.data();
			e = cmd.data() + cmd.size();
			while(*b == ' ' || *b == '\t') ++b;
			if(!*b) continue;
			while(e[-1] == ' ' || e[-1] == '\t') --e;
			cmd = std::string(b, e);
		}

		return cmd;
	}
}

//namespace pkmn
//{
//	class Test
//	{
//		public:
//			void run( );
//		private:
//			nbt::TAG_Compound_ptr_t data_;
//			battle::Battle battle_;
//			struct
//			{
//				dav::UUID id;
//				Pokemon_ptr poke;
//			} trainer_[2];
//	};
//
//	void Test::run(void)
//	{
//		using battle::Events;
//		using battle::Info;
//
//		data_ = nbt::readFile("pokemon.nbt");
//
//		std::cout << "Read data file ..." << std::endl;
//
//		SpeciesManager::Load(data_->getTagList("Species"));
//		std::cout << "Loading species ..." << std::endl;
//		Moves::Load(data_->getTagList("Moves"));
//		std::cout << "Loading moves ..." << std::endl;
//
//		trainer_[0].poke = Pokemon::Generate("charizard", 100);
//		trainer_[1].poke = Pokemon::Generate("charizard", 100);
//
//		std::cout << "Pokemon generated ..." << std::endl;
//
//		battle_.registerTrainers(trainer_[0].id, trainer_[0].poke, trainer_[1].id, trainer_[1].poke);
//
//		std::cout << "Battle initialized.\n" << std::endl;
//		
//		while(battle_.hasEvents())
//		{
//			battle::Event_ptr e = battle_.getEvent();
//
//			switch(e->ID())
//			{
//				case Events::SEND_OUT:
//					if(e->trainer() == trainer_[0].id)
//					{
//						std::cout << "Go, " << trainer_[0].poke->Name() << "!" << std::endl;
//					}
//					else
//					{
//						std::cout << "Opponent send out " << trainer_[1].poke->Name() << "!" << std::endl;
//					}
//					break;
//				case Events::FAINTED:
//					if(e->trainer() == trainer_[0].id)
//					{
//						std::cout << trainer_[0].poke->Name() << " fainted." << std::endl;
//						std::cout << "You lost!" << std::endl;
//						return;
//					}
//					else
//					{
//						std::cout << "Foe " << trainer_[1].poke->Name() << " fainted." << std::endl;
//						std::cout << "You won!" << std::endl;
//						return;
//					}
//					break;
//				case Events::DEMAND:
//					if(e->trainer() == trainer_[0].id)
//					{
//						auto poke = trainer_[0].poke;
//						std::cout << "# " << poke->Name() << ": " << poke->health << "/" << poke->stats[0] << std::endl;
//						std::cout << "# Atk " << poke->stats[1] << "\tDef " << poke->stats[2] << std::endl;
//						std::cout << "# SpA " << poke->stats[3] << "\tSpD " << poke->stats[4] << std::endl;
//						std::cout << "# Spe " << poke->stats[5] << std::endl;
//						for(int i = 0 ; i < 4 ; ++i)
//						{
//							if(poke->moves[i].id.empty()) break;
//							std::cout << "# " << (i + 1) << " - " << Moves::Get(poke->moves[i].id).name << std::endl;
//						}
//						std::cout << "\n> ";
//
//						std::string cmd("");
//						std::cin >> cmd;
//
//						uint mv = 0;
//						if(cmd == "1") mv = 1;
//						else if(cmd == "2") mv = 2;
//						else if(cmd == "3") mv = 3;
//						else if(cmd == "4") mv = 4;
//						else if(cmd == "flee")
//						{
//							std::cout << "You fled the battle" << std::endl;
//							return;
//						}
//
//						if(!mv)
//						{
//							std::cout << "Invalid input. Try again!" << std::endl;
//						}
//						else
//						{
//							--mv;
//							if(trainer_[0].poke->moves[mv].id.empty())
//							{
//								std::cout << "That move doesn't exist. Try again!" << std::endl;
//							}
//							else
//							{
//								battle_.useMove(trainer_[0].id, trainer_[0].poke->id, mv);
//							}
//						}
//					}
//					else
//					{
//						battle_.useMove(trainer_[1].id, trainer_[1].poke->id, 0);
//					}
//					break;
//				case Events::USE_ATTACK:
//					{
//						bool isme = e->trainer() == trainer_[0].id;
//						if(isme) std::cout << trainer_[0].poke->Name();
//						else std::cout << "Foe " << trainer_[1].poke->Name();
//						uint mv = std::dynamic_pointer_cast<battle::UseMove_Event>(e)->move();
//						std::cout << " used " << Moves::Get(trainer_[isme ? 0 : 1].poke->moves[mv].id).name << "." << std::endl;
//						break;
//					}
//				case Events::MISSED:
//					std::cout << "Attack missed." << std::endl;
//					break;
//				case Events::DAMAGE:
//					if(e->trainer() == trainer_[0].id)
//					{
//						uint d = std::dynamic_pointer_cast<battle::Damage_Event>(e)->damage();
//						std::cout << trainer_[0].poke->Name() << " suffered " << d << " damage." << std::endl;
//						trainer_[0].poke->health -= d;
//					}
//					else
//					{
//						uint d = std::dynamic_pointer_cast<battle::Damage_Event>(e)->damage();
//						std::cout << "Foe " << trainer_[1].poke->Name() << " suffered " << d << " damage." << std::endl;
//						trainer_[1].poke->health -= d;
//					}
//					break;
//				case Events::STAT:
//					break;
//				case Events::INFO:
//				{
//					switch(std::dynamic_pointer_cast<battle::Info_Event>(e)->info())
//					{
//						case Info::CHALLENGED:
//							break;
//						case Info::FAILED:
//							std::cout << "But it failed ..." << std::endl;
//							break;
//						case Info::CRITICAL_HIT:
//							std::cout << "Critical hit!" << std::endl;
//							break;
//						case Info::SUPER_EFFECTIVE:
//							std::cout << "It's super effective!" << std::endl;
//							break;
//						case Info::NOT_EFFECTIVE:
//							std::cout << "It's not very effective." << std::endl;
//							break;
//					}
//				}
//					break;
//			}
//		}
//	}
//}

void run_test(void)
{
	pkmn::Battle t;

	std::cout << "Starting simulator ..." << std::endl;

	t.run();
}

