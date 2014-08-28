#include "Editor.h"

Editor::Editor(void) : quitDialog("Do you really want to quit?", [this](bool a) { if(a) quit(); })
{
	cmdNames['q'] = std::string("quit");

	cmds["quit"] = std::bind(&ncurses::Curse::play, &quitDialog);
}

Editor::~Editor(void)
{
}

void Editor::input(int ch)
{
	switch(ch)
	{
		case ':':
			enterCommand();
			return;
	}

	if(static_cast<char>(ch) == ch)
	{
		if(cmdNames.count(static_cast<char>(ch)) > 0)
		{
			command(cmdNames.at(static_cast<char>(ch)));

			return;
		}
	}
}

void Editor::update(int ms)
{
}

void Editor::refresh(void)
{
}

void Editor::command(const std::string& cmd)
{
	std::vector<std::string> params = processCmd(cmd);

	if(params.size() > 0)
	{
		char c = *params.at(0).c_str();

		if(params.at(0).length() == 1 && cmdNames.count(c) > 0)
		{
			params[0] = cmdNames.at(c);
		}

		if(cmds.count(params.at(0)) > 0)
		{
			cmds.at(params.at(0))(params);
		}
	}
}

void Editor::enterCommand(void)
{
	showCursor(true);
	setCursorPos(0, getHeight() - 1);
	clearLine();
	printf(":");
	setInputFunction(ncurses::ReadLine(*this,
		[this](const std::string& s)->void 
		{
			command(s);
		}, 
		[this](void)->void
		{
			setCursorPos(0, 0);
			showCursor(false);
		}));
}

Editor::cmdArgs Editor::processCmd(const std::string& cmd)
{
	std::vector<std::string> out;
	std::istringstream iss(cmd);

	while(iss)
	{
		std::string t;
		iss >> t;
		if(!t.empty()) out.push_back(t);
	}

	return out;
}

