#ifndef PKMN_CONTROLLER_H
#define PKMN_CONTROLLER_H

#include "common.h"
#include <string>
#include <nbt/NBT.h>

namespace pkmn
{
	class Player;
	class Camera;

	class Controller
	{
		public:
			static bool load(const std::string&);
			static void generate(const std::string&);
			static Controller& instance( );
			void update(int);
			void render( ) const;
			~Controller( );
		private:
			Controller(nbt::TAG_Compound::ptr_t);
		private:
			static std::shared_ptr<Controller> controller;
			struct Impl;
			Impl *impl_;
		private:
			Controller(const Controller&) = delete;
			Controller& operator=(const Controller&) = delete;
	};
}

#endif

