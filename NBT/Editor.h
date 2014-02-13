#ifndef EDITOR_H
#define EDITOR_H

#include <iostream>
#include "Curse.h"
#include "ReadLine.h"
#include "Banner.h"

class Editor : public ncurses::Curse
{
	public:
		Editor( );
		~Editor( );
	protected:
		void input(int);
		void update(int);
		void refresh( );
	private:
		void updateBanner(const std::string&);
		ncurses::Banner b;
};

#endif

