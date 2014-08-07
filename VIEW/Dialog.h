#ifndef DIALOG_H
#define DIALOG_H

#include <string>
#include "Screen.h"

class Dialog : public Screen
{
	using Screen::Controls;
	using Screen::modifier_t;

	public:
		Dialog(const std::string&);
	private:
		void i_setScreen(int, int);
		void i_keyPress(Controls, const modifier_t&);
		void i_suspend(bool);
	private:
		std::string msg_;
};

#endif

