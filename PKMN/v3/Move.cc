#include "Move.h"

namespace pkmn
{
	void Moves::Load(nbt::TAG_List_ptr_t list)
	{
		for(auto i(list->begin<nbt::TAG_Compound>()), e(list->end<nbt::TAG_Compound>()) ; i != e ; ++i)
		{
			std::shared_ptr<Move> s = Read(*i);
			Instance().moves_[s->id] = s;
		}
	}

	const Move& Moves::Get(const std::string& sid)
	{
		auto i(Instance().moves_.find(sid));
		if(i == Instance().moves_.end())
			throw std::string("ERR: Move '" + sid + "' doesn't exist!");
		return *i->second;
	}

	std::shared_ptr<Move> Moves::Read(nbt::TAG_Compound_ptr_t tag)
	{
		auto getTarget = [](const std::string& s) -> Target
		{
			     if(s == "Self")     return Target::SELF;
			else if(s == "Opponent") return Target::OPPONENT;
			else throw std::string("ERR: Invalid move target '" + s + "'!");
		};

		std::shared_ptr<Move> move = std::make_shared<Move>();

		move->id = tag->getString("ID");
		move->name = tag->getString("Name");
		move->description = tag->getString("Description");
		move->type = tag->getString("Type");
		
		{
			std::string s(tag->getString("Category"));

			     if(s == "Status")   move->category = Category::STATUS;
			else if(s == "Physical") move->category = Category::PHYSICAL;
			else if(s == "Special")  move->category = Category::SPECIAL;
			else throw std::string("ERR: Invalid move category '" + s + "'!");
		}

		move->power = tag->getInt("Power");
		move->accuracy = tag->getFloat("Accuracy");
		move->pp = tag->getInt("PP");
		move->priority = tag->getInt("Priority");
		move->target = getTarget(tag->getString("Target"));

		move->contact = tag->getByte("MakesContact");

		{
			nbt::TAG_List_ptr_t stats = tag->getTagList("StatChanges");

			for(auto i(stats->begin<nbt::TAG_Compound>()), e(stats->end<nbt::TAG_Compound>()) ; i != e ; ++i)
			{
				Move::StatChanges sc;
				sc.stat = (*i)->getInt("Stat");
				sc.stages = (*i)->getInt("Stages");
				sc.chance = (*i)->getFloat("Chance");
				sc.target = (*i)->hasTag("Target") ? getTarget(tag->getString("Target")) : move->target;
				move->stat_changes.push_back(sc);
			}
		}

		{
			nbt::TAG_List_ptr_t status = tag->getTagList("StatusChanges");

			for(auto i(status->begin<nbt::TAG_Compound>()), e(status->end<nbt::TAG_Compound>()) ; i != e ; ++i)
			{
				Move::StatusChanges sc;
				sc.status = (*i)->getInt("Status");
				sc.chance = (*i)->getFloat("Chance");
				sc.target = (*i)->hasTag("Target") ? getTarget(tag->getString("Target")) : move->target;
				move->status_changes.push_back(sc);
			}
		}

		return move;
	}
}

