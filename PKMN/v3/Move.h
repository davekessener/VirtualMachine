#ifndef PKMN_GAME_MOVE_H
#define PKMN_GAME_MOVE_H

#include <vector>
#include <map>
#include <memory>
#include "common.h"
#include <nbt/NBT.h>

namespace pkmn
{
	enum class Category : BYTE
	{
		STATUS,
		PHYSICAL,
		SPECIAL
	};

	enum class Target : BYTE
	{
		SELF,
		OPPONENT
	};

	struct Move
	{
		struct StatChanges
		{
			uint stat;
			int stages;
			float chance;
		};
		struct StatusChanges
		{
			std::string status;
			float chance;
		};

		std::string id;
		std::string name;
		std::string description;
		std::string type;
		Category category;
		uint power;
		float accuracy;
		uint pp;
		int priority;
		Target target;
		bool contact;
		std::vector<StatChanges> stat_changes;
		std::vector<StatusChanges> status_changes;
	};

	typedef const Move *Move_ptr;

	class Moves
	{
		public:
			static void Load(nbt::TAG_List_ptr_t);
			static const Move& Get(const std::string&);
		private:
			static Moves& Instance( ) { static Moves m; return m; }
			static std::shared_ptr<Move> Read(nbt::TAG_Compound_ptr_t);
		private:
			std::map<std::string, std::shared_ptr<Move>> moves_;
		private:
			Moves( ) = default;
			~Moves( ) = default;
			Moves(const Moves&) = delete;
			Moves& operator=(const Moves&) = delete;
	};
}

#endif

