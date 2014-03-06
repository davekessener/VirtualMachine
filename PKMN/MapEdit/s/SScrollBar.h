#ifndef _SSCROLLBAR_H
#define _SSCROLLBAR_H

#include <array>
#include "Surface.h"
#include "SButton.h"

class SScrollBar : public Surface
{
	protected:
	typedef std::array<std::string, 5> names_arr;

	public:
		SScrollBar(Image *, int, int, int, int, int, names_arr);
		virtual ~SScrollBar( );
		inline int getRelativePosition( ) { return position; }
		void mouseDown(button_t, int, int);
	protected:
		void draw( );
		virtual names_arr getIconNames( ) = 0;
		void calculatePosition(int, int);
	private:
		void setPosition(int);
		int position, range;
		Image *cursor[2], *fill;
		SButton *incr, *decr;
};

#endif

