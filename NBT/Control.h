#ifndef NBTEDIT_CONTROL_H
#define NBTEDIT_CONTROL_H

#include <memory>

class Control
{
	public:
		inline void render( ) const { i_doRender(); }
		inline void input(int i) { i_doInput(i); }
	private:
		virtual void i_doRender( ) const = 0;
		virtual void i_doInput(int) = 0;
};

typedef std::shared_ptr<Control> Control_ptr;

#endif

