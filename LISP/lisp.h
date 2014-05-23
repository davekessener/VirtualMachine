#ifndef LISP_H
#define LISP_H

#include <iostream>
#include <vector>
#include "common.h"
#include "Reader.h"

namespace lisp
{
	class Lisp
	{
		public:
			Lisp( );
			~Lisp( );
			int run(const std::vector<std::string>&);
		private:

		private:
			Lisp(const Lisp&);
			Lisp& operator=(const Lisp&);
	};
}

#endif

