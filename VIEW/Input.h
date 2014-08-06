#ifndef INPU_H
#define INPUT_H

#include "Screen.h"

class Input : public Screen
{
	protected:
	using Screen::Controls;
	using Screen::modifier_t;

	public:
		Input( );
		virtual ~Input( ) noexcept;
	protected:
		virtual void setString(const std::string&, int = -1, int = -1);
	private:
		void i_setScreen(int, int);
		void i_suspend(bool);
		void i_keyPress(Controls, const modifier_t&);
	private:
		struct Impl;
		Impl *impl_;
};

#endif

