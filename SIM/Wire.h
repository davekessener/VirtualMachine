#ifndef SIM_WIRE_H
#define SIM_WIRE_H

#include <vector>
#include <cassert>
#include <memory>
#include "Sim.h"
#include "Conductable.h"

namespace sim
{
	class Wire
	{
		typedef std::vector<const Conductable *> cond_vec_t;
		public:
		typedef std::shared_ptr<Wire> Wire_ptr;

		public:
			Wire( );
			~Wire( );
			void connect(const Conductable&);
			void disconnect(const Conductable&);
			V_t out( ) const;
			void update( );
		protected:
		private:
			cond_vec_t::iterator find(const Conductable&);

			cond_vec_t pins_;
			V_t v_;
	};
}

#endif

