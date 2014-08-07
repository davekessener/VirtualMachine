#ifndef FILEINPUT_H
#define FILEINPUT_H

#include "Input.h"

class FileInput : public Input
{
	using Input::Controls;
	using Input::modifier_t;

	public:
		FileInput( );
		virtual ~FileInput( ) noexcept;
	private:
		void i_setScreen(int, int);
		void i_keyPress(Controls, const modifier_t&);
	private:
		struct Impl;
		Impl *impl_;
};

#endif

