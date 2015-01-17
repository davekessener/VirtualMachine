#ifndef PKMN_GAME_TYPES_H
#define PKMN_GAME_TYPES_H

#include <vector>
#include <map>
#include <initializer_list>
#include "common.h"

namespace pkmn
{
	struct Type
	{
		std::string id;
		std::string name;
		std::vector<std::string> strong, weak, immune;
	};

	class Types
	{
		public:
			static float Modifier(const std::string& sid_a, const std::string& sid_b)
			{
				const Type &a(Get(sid_a));

				for(const auto& t : a.immune)
				{
					if(t == sid_b) return 0.0;
				}

				for(const auto& t : a.weak)
				{
					if(t == sid_b) return 0.5;
				}

				for(const auto& t : a.strong)
				{
					if(t == sid_b) return 2.0;
				}

				return 1.0;
			}
			static bool WillFail(const std::string& sid_a, const std::string& sid_b)
			{
				const Type &a(Get(sid_a));

				for(const auto& t : a.immune)
				{
					if(t == sid_b) return true;
				}

				return false;
			}
			static const Type& Get(const std::string& sid)
			{
				auto i(Instance().types_.find(sid));
				if(i == Instance().types_.end()) throw std::string("ERR: Type '" + sid + "' unknown!");
				return *i->second;
			}
		private:
			static Types& Instance( ) { static Types t; return t; }
		private:
			void add(const std::string& id, const std::string& name,
					 std::initializer_list<std::string>&& strong,
					 std::initializer_list<std::string>&& weak,
					 std::initializer_list<std::string>&& immune)
			{
				Type t;
				t.id = id;
				t.name = name;
				t.strong = strong;
				t.weak = weak;
				t.immune = immune;
				types_[t.id] = t;
			}
		private:
			std::map<std::string, std::shared_ptr<Type>> types_;
		private:
			Types( )
			{
				add("???", "???", {}, {}, {});
				add("normal", "Normal", {}, {"rock", "steel"}, {"ghost"});
				add("fighting", "Fighting", {"normal", "rock", "steel", "ice", "dark"}, {"flying", "poison", "bug", "psychic", "fairy"}, {"ghost"});
				add("flying", "Flying", {"fighting", "bug", "grass"}, {"rock", "steel", "electric"}, {});
				add("poison", "Poison", {"grass", "fairy"}, {"poison", "ground", "rock", "ghost"}, {"steel"});
				add("ground", "Ground", {"poison", "rock", "steel", "fire"}, {"bug", "grass"}, {"flying"});
				add("rock", "Rock", {"flying", "bug", "fire", "ice"}, {"fighting", "ground", "steel"}, {});
				add("bug", "Bug", {"grass", "psychic", "dark"}, {"fighting", "flying", "poison", "ghost", "steel", "fire", "fairy"}, {});
				add("ghost", "Ghost", {"ghost", "psychic"}, {"dark"}, {"normal"});
				add("steel", "Steel", {"rock", "ice", "fairy"}, {"steel", "fire", "water", "electric"}, {});
				add("fire", "Fire", {"bug", "steel", "grass", "ice"}, {"rock", "fire", "water", "dragon"}, {});
				add("water", "Water", {"ground", "rock", "fire"}, {"water", "grass", "dragon"}, {});
				add("grass", "Grass", {"ground", "rock", "water"}, {"flying", "poison", "bug", "steel", "fire", "grass", "dragon"}, {});
				add("electric", "Electric", {"flying", "water"}, {"grass", "electric", "dragon"}, {"ground"});
				add("psychic", "Psychic", {"fighting", "poison"}, {"steel", "psychic"}, {"dark"});
				add("ice", "Ice", {"flying", "ground", "grass", "dragon"}, {"steel", "fire", "water", "ice"}, {});
				add("dragon", "Dragon", {"dragon"}, {"steel"}, {"fairy"});
				add("dark", "Dark", {"ghost", "psychic"}, {"fighting", "dark", "fairy"}, {});
				add("fairy", "Fairy", {"fighting", "dragon", "dark"}, {"poison", "steel", "fire"}, {});
			}
			~Types( ) = default;
			Types(const Types&) = delete;
			Types& operator=(const Types&) = delete;
	};
}

#endif

