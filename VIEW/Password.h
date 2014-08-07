#ifndef PASSWORD_H
#define PASSWORD_H

#include "Input.h"

class Password : public Input
{
	using Input::Controls;
	using Input::modifier_t;

	public:
		Password( );
		virtual ~Password( ) noexcept;
	private:
		void i_setScreen(int, int);
		void i_keyPress(Controls, const modifier_t&);
	private:
		struct Impl;
		Impl *impl_;
};

#endif

