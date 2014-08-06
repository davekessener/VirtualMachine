#ifndef SCREEN_H
#define SCREEN_H

#include <set>
#include <functional>
#include <cassert>
#include "sdl.h"

class Screen
{
	protected:
	typedef sdl::Controls Controls;
	typedef std::set<Controls> modifier_t;
	typedef std::function<void(void)> render_fn;

	public:
		Screen( ) : alive_(true) { }
		virtual ~Screen( ) noexcept { }
		void setScreen(int w, int h) { i_setScreen(w, h); }
		bool ready( ) const { return i_ready(); }
		bool alive( ) const { return alive_; }
		void render( ) const { assert(i_ready()); render_(); }
		void suspend(bool s = true) { i_suspend(s); }
		void keyPress(sdl::Controls c, const std::set<sdl::Controls>& m) { i_keyPress(c, m); }
	protected:
		void setRender(const render_fn& r) { render_ = r; }
		void kill( ) { alive_ = false; }
	private:
		virtual void i_setScreen(int, int) = 0;
		virtual bool i_ready( ) const { return static_cast<bool>(render_); };
		virtual void i_suspend(bool s) = 0;
		virtual void i_keyPress(Controls, const modifier_t&) = 0;
	private:
		bool alive_;
		render_fn render_;
};

#endif

