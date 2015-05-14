#ifndef DAV_LISP_INTERNAL_H
#define DAV_LISP_INTERNAL_H

namespace lisp
{
	class Cell
	{
		using Cell_ptr = std::shared_ptr<Cell>;

		public:
		private:
			Cell_ptr car, cdr;
	};
}

#endif

