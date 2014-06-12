#ifndef SIM_PIN_H
#define SIM_PIN_H

#include "Sim.h"
#include "Conductable.h"
#include "Wire.h"

namespace sim
{
	class Pin : public Conductable
	{
		public:
			Pin( );
			~Pin( );
			V_t out( ) const;
			void in(const V_t&);
			void connect(Wire&);
			V_t passive( ) const;
		protected:
		private:
			V_t v_;
			Wire *w_;
	};
}

#endif

