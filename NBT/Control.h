#ifndef NBTEDIT_CONTROL_H
#define NBTEDIT_CONTROL_H

#include <memory>

class Control
{
	public:
	typedef std::shared_ptr<Control> Control_ptr;

	public:
		inline void render( ) const { self().i_doRender(); }
		inline void input(int i) { self().i_doInput(i); }
		inline void pushControl(Control_ptr c) { self().super_ = c; }
		inline Control& self( ) { return static_cast<bool>(super_) ? super_->self() : *this; }
		inline const Control& self( ) const { return static_cast<bool>(super_) ? super_->self() : *this; }
	private:
		virtual void i_doRender( ) const = 0;
		virtual void i_doInput(int) = 0;
	private:
		Control_ptr super_;
};

typedef Control::Control_ptr Control_ptr;

#endif

