#include "common.h"
#include "battle/Battle.h"
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
			bool running_;
			struct
			{
				dav::UUID id;
				Pokemon_ptr poke;
			} trainer_[2];
	};

	void run_test(void)
	{
		Test t;

		t.run();
	}

	void Test::run(void)
	{
		data_ = nbt::readFile("pokemon.nbt");

		Species::Load(data->getTagList("Species"));
		Moves::Load(data->getTagList("Moves"));

		trainer_[0].poke = Pokemon::Generate("charizard", 100);
		trainer_[1].poke = Pokemon::Generate("charizard", 100);

		battle_.registerTrainer(trainer_[0].id, trainer_[0].poke, trainer_[1].id, trainer_[1].poke);
		
		while(battle_.hasEvents())
		{
			battle::Event_ptr e = battle_.getEvent();

			switch(e->ID())
			{
				case Events::SEND_OUT:
					break;
				case Events::DEMAND:
					break;
				case Events::USE_ATTACK:
					break;
				case Events::MISSED:
					break;
				case Events::DAMAGE:
					break;
				case Events::STAT:
					break;
				case Events::INFO:
				{
					switch(std::dynamic_pointer_cast<Info_Event>(e)->info())
					{
						case Info::CHALLENGED:
							break;
						case Info::FAILED:
							break;
						case Info::SUPER_EFFECTIVE:
							break;
						case Info::NOT_EFFECTIVE:
							break;
					}
				}
					break;
			}
		}


		running_ = true;

		while(running_)
		{
			printStatus();
			std::string cmd = readCmd();

			if(cmd == "1")
			{
				attack(0);
			}
			else if(cmd == "2")
			{
				attack(1);
			}
			else if(cmd == "3")
			{
				attack(2);
			}
			else if(cmd == "4")
			{
				attack(3);
			}
			else if(cmd == "flee")
			{
				running_ = false;
			}
		}
	}

	void Test::attack(uint idx)
	{
		if(trainer_[0].poke->moves[idx].id.empty())
		{
			std::cout << "Invalid move. Try again." << std::endl;
		}
		else
		{
			battle_.useMove(trainer_[0].id, trainer_[0].poke->id, trainer_[0].poke->moves[idx].id);
			battle_.useMove(trainer_[1].id, trainer_[1].poke->id, trainer_[1].poke->moves[0].id);
		}
	}
}

