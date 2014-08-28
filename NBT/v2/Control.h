#ifndef NBTEDIT_CONTROL_H
#define NBTEDIT_CONTROL_H

#include <memory>
#include <dav/Logger.h>

class Control : public std::enable_shared_from_this<Control>
{
	public:
	typedef std::shared_ptr<Control> Control_ptr;
	typedef std::weak_ptr<Control> Control_wptr;

	public:
		inline void render( ) const { i_doRender(); if(super_) super_->render(); }
		inline void input(int i) { self().i_doInput(i); }
		inline void pushControl(Control_ptr c) { LOG("[%p]: Pushed %p onto %p", this, &*c, &self()); c->setParent(self().shared_from_this()); self().super_ = c; }
		inline void popControl( ) { if(self().hasParent()) { LOG("[%p]: Popped %p from %p", this, &*self().parent()->super_, &self()); self().parent()->super_.reset(); } }
		inline void setParent(Control_ptr c) { parent_ = c; }
		inline Control& self( ) { return static_cast<bool>(super_) ? super_->self() : *this; }
		inline const Control& self( ) const { return static_cast<bool>(super_) ? super_->self() : *this; }
		inline bool hasParent( ) const { return static_cast<bool>(parent_.lock()); }
		inline Control_ptr parent( ) { return parent_.lock(); }
	private:
		virtual void i_doRender( ) const = 0;
		virtual void i_doInput(int) = 0;
	private:
		Control_ptr super_;
		Control_wptr parent_;
};

typedef Control::Control_ptr Control_ptr;

#endif

