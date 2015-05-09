#ifndef DAV_VM_PERIPHERY_H
#define DAV_VM_PERIPHERY_H

#include <memory>
#include "common.h"

namespace vm
{
	class Periphery
	{
		public:
			virtual void activate( ) = 0;
			virtual void deactivate( ) = 0;
			virtual void accept(WORD) = 0;
			virtual WORD retrieve( ) = 0;
		private:
	};

	using Periphery_ptr = std::shared_ptr<Periphery>;
}

#endif

