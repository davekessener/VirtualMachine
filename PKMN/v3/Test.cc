#include "common.h"
#include "battle/Battle.h"
#include "battle/Event.hpp"
#include <nbt/NBT.h>

namespace pkmn
{
	class Test
	{
		public:
			void run( );
		private:
			nbt::TAG_Compound_ptr_t data_;
			battle::Battle battle_;
			struct
			{
				dav::UUID id;
				Pokemon_ptr poke;
			} trainer_[2];
	};

	void Test::run(void)
	{
		using battle::Events;
		using battle::Info;

		data_ = nbt::readFile("pokemon.nbt");

		std::cout << "Read data file ..." << std::endl;

		SpeciesManager::Load(data_->getTagList("Species"));
		std::cout << "Loading species ..." << std::endl;
		Moves::Load(data_->getTagList("Moves"));
		std::cout << "Loading moves ..." << std::endl;

		trainer_[0].poke = Pokemon::Generate("charizard", 100);
		trainer_[1].poke = Pokemon::Generate("charizard", 100);

		std::cout << "Pokemon generated ..." << std::endl;

		battle_.registerTrainers(trainer_[0].id, trainer_[0].poke, trainer_[1].id, trainer_[1].poke);

		std::cout << "Battle initialized.\n" << std::endl;
		
		while(battle_.hasEvents())
		{
			battle::Event_ptr e = battle_.getEvent();

			switch(e->ID())
			{
				case Events::SEND_OUT:
					if(e->trainer() == trainer_[0].id)
					{
						std::cout << "Go, " << trainer_[0].poke->Name() << "!" << std::endl;
					}
					else
					{
						std::cout << "Opponent send out " << trainer_[1].poke->Name() << "!" << std::endl;
					}
					break;
				case Events::FAINTED:
					if(e->trainer() == trainer_[0].id)
					{
						std::cout << trainer_[0].poke->Name() << " fainted." << std::endl;
						std::cout << "You lost!" << std::endl;
						return;
					}
					else
					{
						std::cout << "Foe " << trainer_[1].poke->Name() << " fainted." << std::endl;
						std::cout << "You won!" << std::endl;
						return;
					}
					break;
				case Events::DEMAND:
					if(e->trainer() == trainer_[0].id)
					{
						auto poke = trainer_[0].poke;
						std::cout << "# " << poke->Name() << ": " << poke->health << "/" << poke->stats[0] << std::endl;
						std::cout << "# Atk " << poke->stats[1] << "\tDef " << poke->stats[2] << std::endl;
						std::cout << "# SpA " << poke->stats[3] << "\tSpD " << poke->stats[4] << std::endl;
						std::cout << "# Spe " << poke->stats[5] << std::endl;
						for(int i = 0 ; i < 4 ; ++i)
						{
							if(poke->moves[i].id.empty()) break;
							std::cout << "# " << (i + 1) << " - " << Moves::Get(poke->moves[i].id).name << std::endl;
						}
						std::cout << "\n> ";

						std::string cmd("");
						std::cin >> cmd;

						uint mv = 0;
						if(cmd == "1") mv = 1;
						else if(cmd == "2") mv = 2;
						else if(cmd == "3") mv = 3;
						else if(cmd == "4") mv = 4;
						else if(cmd == "flee")
						{
							std::cout << "You fled the battle" << std::endl;
							return;
						}

						if(!mv)
						{
							std::cout << "Invalid input. Try again!" << std::endl;
						}
						else
						{
							--mv;
							if(trainer_[0].poke->moves[mv].id.empty())
							{
								std::cout << "That move doesn't exist. Try again!" << std::endl;
							}
							else
							{
								battle_.useMove(trainer_[0].id, trainer_[0].poke->id, mv);
							}
						}
					}
					else
					{
						battle_.useMove(trainer_[1].id, trainer_[1].poke->id, 0);
					}
					break;
				case Events::USE_ATTACK:
					{
						bool isme = e->trainer() == trainer_[0].id;
						if(isme) std::cout << trainer_[0].poke->Name();
						else std::cout << "Foe " << trainer_[1].poke->Name();
						uint mv = std::dynamic_pointer_cast<battle::UseMove_Event>(e)->move();
						std::cout << " used " << Moves::Get(trainer_[isme ? 0 : 1].poke->moves[mv].id).name << "." << std::endl;
						break;
					}
				case Events::MISSED:
					std::cout << "Attack missed." << std::endl;
					break;
				case Events::DAMAGE:
					if(e->trainer() == trainer_[0].id)
					{
						uint d = std::dynamic_pointer_cast<battle::Damage_Event>(e)->damage();
						std::cout << trainer_[0].poke->Name() << " suffered " << d << " damage." << std::endl;
						trainer_[0].poke->health -= d;
					}
					else
					{
						uint d = std::dynamic_pointer_cast<battle::Damage_Event>(e)->damage();
						std::cout << "Foe " << trainer_[1].poke->Name() << " suffered " << d << " damage." << std::endl;
						trainer_[1].poke->health -= d;
					}
					break;
				case Events::STAT:
					break;
				case Events::INFO:
				{
					switch(std::dynamic_pointer_cast<battle::Info_Event>(e)->info())
					{
						case Info::CHALLENGED:
							break;
						case Info::FAILED:
							std::cout << "But it failed ..." << std::endl;
							break;
						case Info::SUPER_EFFECTIVE:
							std::cout << "It's super effective!" << std::endl;
							break;
						case Info::NOT_EFFECTIVE:
							std::cout << "It's not very effective." << std::endl;
							break;
					}
				}
					break;
			}
		}
	}
}

void run_test(void)
{
	pkmn::Test t;

	std::cout << "Starting simulator ..." << std::endl;

	t.run();
}

