#ifndef _SSCROLLABLE_H
#define _SSCROLLABLE_H

#include "Surface.h"
#include "SScrollBar.h"
#include "SVScrollBar.h"
#include "SHScrollBar.h"

class SScrollable : public Surface
{
	public:
		SScrollable(Image *, int, int, int, int, std::pair<int, int>);
		~SScrollable( );
	protected:
		void draw( );
		virtual void draw(int, int) = 0;
		virtual std::pair<int, int> getScrollRanges( ) = 0;
	private:
		SScrollBar *vscroll, *hscroll;
};

#endif

