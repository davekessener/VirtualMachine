#ifndef PKMN_GAME_NATURES_H
#define PKMN_GAME_NATURES_H

#include <map>
#include <algorithm>
#include "common.h"
#include "UUID.hpp"

namespace pkmn
{
	struct Nature
	{
		std::string name;
		int inc, dec;
	};

	class Natures
	{
		public:
			static std::string Random( )
			{
				auto i(Instance().natures_.begin());
				std::advance(i, dav::UUID::rand<uint>() % 25);
				return i->second.name;
			}
			double Multiplier(const std::string& sid, uint stat)
			{
				auto i(Instance().natures_.find(sid));
				if(i == Instance().natures_.end()) throw std::string("ERR: Unknown nature '" + sid + "'!");
				return stat > 0 && stat < 6 ? (i->second.inc == stat ? 1.1 : (i->second.dec == stat ? 0.9 : 1.0)) : 1.0;
			}
		private:
			static Natures& Instance( ) { static Natures n; return n; }
			void add(const std::string& name, int inc, int dec)
			{
				Nature n;
				n.name = name;
				n.inc = inc;
				n.dec = dec;
				if(inc == dec) n.inc = n.dec = 0;
				natures_[n.name] = n;
			}
		private:
			std::map<std::string, Nature> natures_;
		private:
			Natures( )
			{
				add("adamant", 1, 3);
				add("bashful", 3, 3);
				add("bold", 2, 1);
				add("brave", 1, 5);
				add("calm", 4, 1);
				add("careful", 4, 3);
				add("docile", 2, 2);
				add("gentle", 4, 2);
				add("hardy", 1, 1);
				add("hasty", 5, 2);
				add("impish", 2, 3);
				add("jolly", 5, 3);
				add("lax", 2, 4);
				add("lonely", 1, 2);
				add("mild", 3, 2);
				add("modest", 3, 1);
				add("naive", 5, 4);
				add("naughty", 1, 4);
				add("quiet", 3, 5);
				add("quirky", 4, 4);
				add("rash", 3, 4);
				add("relaxed", 2, 5);
				add("sassy", 4, 5);
				add("serious", 5, 5);
				add("timid", 5, 1);
			}
			~Natures( ) = default;
			Natures(const Natures&) = delete;
			Natures& operator=(const Natures&) = delete;
	};
}

#endif

