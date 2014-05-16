#ifndef SIM_AXIOMTEMPLATE_H
#define SIM_AXIOMTEMPLATE_H

#include "Sim.h"
#include "Template.h"

namespace sim
{
	template<typename T>
	class AxiomTemplate : public Template
	{
		public:
			AxiomTemplate( );
			virtual Chip::Chip_ptr instantiate( ) const;
		protected:
		private:
	};

	template<typename T>
	AxiomTemplate<T>::AxiomTemplate(void) : Template(std::vector<std::string>{"_"})
	{
	}

	template<typename T>
	Chip::Chip_ptr AxiomTemplate<T>::instantiate(void) const
	{
		return Chip::Chip_ptr(new T);
	}
}

#endif

