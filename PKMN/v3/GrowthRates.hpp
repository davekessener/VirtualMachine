#ifndef PKMN_GAME_GROWTHRATES_H
#define PKMN_GAME_GROWTHRATES_H

#include <vector>
#include <functional>
#include "common.h"

namespace pkmn
{
	class GrowthRate
	{
		typedef std::function<uint(uint)> exp_fn;

		public:
			const std::string& name( ) const { return name_; }
			uint nextLevel(uint lvl) const { return lvl < 99 ? experience(lvl + 1) - experience(lvl) : 0; }
			uint experience(uint lvl) const { return lvl < 100 ? exp_(lvl + 1) : 0; }
		public:
			GrowthRate( ) : name_("#ERR#") { }
			GrowthRate(const std::string& n, exp_fn e) : name_(n), exp_(e) { }
			GrowthRate(const GrowthRate&) = default;
			~GrowthRate( ) = default;
			GrowthRate& operator=(const GrowthRate&) = default;
		private:
			std::string name_;
			exp_fn exp_;
	};

	class GrowthRates
	{
		public:
			static const GrowthRate& Get(uint i) { return Instance().rates_.at(i); }
		private:
			static GrowthRates& Instance( ) { static GrowthRates gr; return gr; }
		private:
			GrowthRates( )
			{
				rates_.emplace_back("Erratic", [](uint lvl) -> uint
				{
					if(lvl <= 50) return lvl * lvl * lvl * (100 - lvl) / 50;
					else if(lvl <= 68) return lvl * lvl * lvl * (150 - lvl) / 100;
					else if(lvl <= 98) return lvl * lvl * lvl * ((1911 - 10 * lvl) / 3) / 500;
					else return lvl * lvl * lvl * (160 - lvl) / 100;
				});
				rates_.emplace_back("Fast", [](uint lvl) -> uint
				{
					return 4 * lvl * lvl * lvl / 5;
				});
				rates_.emplace_back("MediumFast", [](uint lvl) -> uint
				{
					return lvl * lvl * lvl;
				});
				rates_.emplace_back("MediumSlow", [](uint lvl) -> uint
				{
					return 6 * lvl * lvl * lvl / 5 - 15 * lvl * lvl + 100 * lvl - 140;
				});
				rates_.emplace_back("Slow", [](uint lvl) -> uint
				{
					return 5 * lvl * lvl * lvl / 4;
				});
				rates_.emplace_back("Fluctuating", [](uint lvl) -> uint
				{
					if(lvl <= 15) return lvl * lvl * lvl * ((((lvl + 1) / 3) + 24) / 50);
					else if(lvl <= 36) return lvl * lvl * lvl * ((lvl + 14) / 50);
					else return lvl * lvl * lvl * (((lvl / 2) + 32) / 50);
				});
			}
			~GrowthRates( ) = default;
			GrowthRates(const GrowthRates&) = delete;
			GrowthRates& operator=(const GrowthRates&) = delete;
		private:
			std::vector<GrowthRate> rates_;
	};
}

#endif

