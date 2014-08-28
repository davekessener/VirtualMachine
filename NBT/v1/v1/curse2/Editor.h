#ifndef EDITOR_H
#define EDITOR_H

#include <iostream>
#include <functional>
#include <vector>
#include <map>
#include <sstream>
#include <cstring>
#include "Curse.h"
#include "ReadLine.h"
#include "YesNoDialog.h"

class Editor : public ncurses::Curse
{
	typedef std::vector<std::string> cmdArgs;
	typedef std::map<char, std::string> cmdNamesMap;
	typedef std::map<std::string, std::function<void(cmdArgs&)>> cmdMap;

	public:
		Editor( );
		~Editor( );
	protected:
		void input(int);
		void update(int);
		void refresh( );
	private:
		void enterCommand(void);
		void command(const std::string&);
		cmdNamesMap cmdNames;
		cmdMap cmds;
		ncurses::YesNoDialog quitDialog;

		static cmdArgs processCmd(const std::string&);
};

#endif

