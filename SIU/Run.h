#ifndef SIU_RUN_H
#define SIU_RUN_H

#include <iostream>
#include <string>
#include <vector>
#include "OnExit.h"

namespace siu
{
	class Run
	{
		public:
			Run( );
			int run(int, char **);
		protected:
			virtual void init( );
			virtual void preprocess(const std::vector<std::string>&);
			virtual int doRun( );
			virtual void cleanup( );
		private:
	};
}

#endif

